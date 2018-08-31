/*

Copyright (C) 2017, 2018 Ivan Kniazkov

This file is part of interpreter of programming language
codenamed "Goat" ("Goat interpreter").

Goat interpreter is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Goat interpreter is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Goat interpreter.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Launcher.h"
#include "Scanner.h"
#include "Parser.h"
#include "Thread.h"
#include "BuiltIn.h"
#include "Utils.h"
#include "Platform.h"
#include "SourceStream.h"
#include "StandartOutputStream.h"
#include "StandartErrorStream.h"
#include "StandartInputStream.h"
#include "Memory.h"
#include "Object.h"
#include "Console.h"
#include "SourceString.h"
#include "WideStringBuilder.h"
#include "FileName.h"
#include "Resource.h"

namespace goat {

	int Launcher::run(Source *src, Environment *env, Scope *scope, Root **proot, Options *opt) {
		Scanner scan(src);
		bool errors = false;
		long long steps = 0;

		try {
			Parser::Options popt;
			popt.path = opt->path;
			popt.libs = &opt->libs;
			Root* root = Parser::parse(&scan, *proot, &popt);
			*proot = root;
			new Thread(root, scope, nullptr);

			if (!opt->debug) {
				while (Thread::current != nullptr) {
					if (Thread::current->step()) {
						steps++;
						// garbage collection
						opt->gc->collect();
						// next thread
						Thread::current = Thread::current->next;
						if (Thread::current == nullptr) {
							Thread::current = ThreadList::global.first;
						}
					}
					else {
						delete Thread::current;
					}
				}
			}
			else {
				InputStream<wchar> *input = StandartInputStream::getInstance();
				OutputStream<wchar> *output = StandartOutputStream::getInstance();
				Console console(input, output, L'\n', L"\r");

				while (Thread::current != nullptr) {
					if (Thread::current->state) {
						Token *tok = Thread::current->state->token();
						if (tok) {
							WideString strMode;
							bool stop = false;
							State::DebugMode mode = Thread::current->state->stop();
							switch (Thread::current->mode) {
								case State::DebugMode::SKIP:
									break;
								case State::DebugMode::OUT:
									if (mode == State::DebugMode::BREAKPOINT) {
										Thread::current->mode = State::DebugMode::OVER;
										strMode = Resource::w_step;
										stop = true;
									}
									else if ((mode == State::DebugMode::OVER || mode == State::DebugMode::INTO) && Thread::current->level > Thread::current->state->level) {
										strMode = Resource::w_out;
										stop = true;
									}
									break;
								case State::DebugMode::INTO:
									if (mode == State::DebugMode::BREAKPOINT) {
										Thread::current->mode = State::DebugMode::OVER;
										strMode = Resource::w_step;
										stop = true;
									}
									else if (mode == State::DebugMode::OVER) {
										strMode = Resource::w_into;
										stop = true;
									}
									else if (mode == State::DebugMode::INTO) {
										Thread::current->mode = State::DebugMode::OVER;
										Thread::current->level++;
									}
									break;
								case State::DebugMode::OVER:
									if (mode == State::DebugMode::BREAKPOINT ||
										(mode == State::DebugMode::OVER && Thread::current->level >= Thread::current->state->level))
									{
										strMode = Resource::w_step;
										stop = true;
									}
									break;
								case State::DebugMode::BREAKPOINT:
									if (mode == State::DebugMode::BREAKPOINT) {
										Thread::current->mode = State::DebugMode::OVER;
										strMode = Resource::w_step;
										stop = true;
									}
									break;
							}

							if (stop) {
								Thread::current->level = Thread::current->state->level;
								console.write((WideStringBuilder() <<
									L"\n> " << tok->loc->toString() << L": " << tok->toString() << 
									L"\n[" << strMode << L"] "
								).toWideString());

								while (true) {
									console.write(L"? ");
									WideString cmd = console.read();
									unsigned int len = cmd.len();
									if (len == 0) {
										break;
									}
									else if (cmd.len() > 1 && cmd[0] == L'$') {
										String name = cmd.subString(1, len - 1).toString();
										Object *obj = Thread::current->state->scope->find(Object::createIndex(name));
										if (obj) {
											console.write((WideStringBuilder() << 
												name << L": " << obj->toWideStringNotation() << L'\n'
											).toWideString());
										}
									}
									else if (cmd == Resource::w_continue || cmd == L"c") {
										Thread::current->mode = State::DebugMode::BREAKPOINT;
										break;
									}
									else if (cmd == Resource::w_step || cmd == L"s") {
										Thread::current->mode = State::DebugMode::OVER;
										break;
									}
									else if (cmd == Resource::w_into || cmd == L"i") {
										Thread::current->mode = State::DebugMode::INTO;
										break;
									}
									else if (cmd == Resource::w_out || cmd == L"o") {
										Thread::current->mode = State::DebugMode::OUT;
										break;
									}
									else {
										console.write(L"unknown command.\n");
									}
								}
							}
						}
						Thread::current->step();
						steps++;
						// garbage collection
						opt->gc->collect();
						// next thread
						Thread::current = Thread::current->next;
						if (Thread::current == nullptr) {
							Thread::current = ThreadList::global.first;
						}
					}
					else {
						delete Thread::current;
					}
				}
			}
		}
		catch (Exception &e) {
			RawString error = e.toRawString();
			if (env->err) {
				wchar *c = error.data;
				while(*c) {
					env->err->write(*c);
					c++;
				}
			}
			error.free();
			errors = true;
		}

		return errors ? 1 : 0;
	}

	int Launcher::runCmdLine(int argc, char **argv) {
		Options opt;

		String program;
		int i;
		
		for (i = 1; i < argc; i++) {
			char *arg = argv[i];
			if (arg[0] == '-' && arg[1] == '-') {
				// argument for interpreter
				if (Utils::strCmp(arg + 2, "gc=", 3) == 0) {
					// choose garbage collector
					if (Utils::strCmp(arg + 5, "debug") == 0) {
						opt.gc = GarbageCollector::debug();
					}
					else if (Utils::strCmp(arg + 5, "serial") == 0) {
						opt.gc = GarbageCollector::serial();
					}
					else if (Utils::strCmp(arg + 5, "parallel") == 0) {
						opt.gc = GarbageCollector::parallel();
					}
				}
				else if (Utils::strCmp(arg + 2, "lib=", 4) == 0) {
					// parse library path
					String libs = arg + 6;
					libs.split(';', opt.libs);
				}
				else if (Utils::strCmp(arg + 2, "debug") == 0) {
					// debug mode
					opt.debug = true;
				}
			}
			else {
				if (arg[0] != '-' && program == nullptr) {
					program = arg;
				}
			}
		}

		if (!opt.gc) {
			opt.gc = GarbageCollector::parallel();
		}

		if (program == "") {
			Utils::printErr(L"no input file.\n");
			return 1;
		}

		int ret = 1;
		Root *root = nullptr;

		program = FileName::normalize(program);
		opt.path = FileName::extractPath(program);
		if (opt.path == "") {
			opt.path = FileName::s_currentPath;
		}

		try {
			Platform::FileReader reader(program);
			SourceStream source(&reader);
			Environment env;
			env.out = StandartOutputStream::getInstance();
			env.err = StandartErrorStream::getInstance();
			ret = run(&source, &env, BuiltIn::create(&env)->clone(), &root, &opt);
		}
		catch (Exception &ex) {
			RawString error = ex.toRawString();
			Utils::printErr(error.data);
			error.free();
		}
		ObjectList::global.sweep();
		if (root) {
			root->free();
		}
		return ret;
	}

	int Launcher::interactive() {
		InputStream<wchar> *input = StandartInputStream::getInstance();
		OutputStream<wchar> *output = StandartOutputStream::getInstance();
		OutputStream<wchar> *errors = StandartErrorStream::getInstance();
		
		Console console(input, output, L'\n', L"\r");
		
		Environment env;
		env.out = output;
		env.err = errors;
		
		Scope *scope = BuiltIn::create(&env)->clone();
		
		Root *root = nullptr;
		
		WideStringBuilder program;
		
		bool prompt = true;

		Options opt;
		opt.gc = GarbageCollector::parallel();

		while (true) {
			console.write(prompt ? (wchar*)L"? " : (wchar*)L"  ");
			WideString line = console.read();
			if (line != L"") {
				if (line == L"exit") {
					break;
				}
				if (line[line.len() - 1] == L'\\') {
					program << line.subString(0, line.len() - 1);
					prompt = false;
				}
				else {
					program << line;
					SourceString source(program.toWideString().toString());
					program.clear();
					prompt = true;
					run(&source, &env, scope, &root, &opt);
					console.write(L'\n');
				}
			}
		}
		ObjectList::global.sweep();
		if (root) {
			root->free();
		}
		return 0;
	}

}
