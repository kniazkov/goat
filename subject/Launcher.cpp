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
			new Thread(root, scope);
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
			output->write(prompt ? (wchar*)L"? " : (wchar*)L"  ");
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
					output->write(L'\n');
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
