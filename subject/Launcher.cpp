/*

Copyright (C) 2017 Ivan Kniazkov

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

namespace goat {

	static const long long threshold = 16 * 1024 * 1024;

	int Launcher::run(Source *src, Environment *env, Scope *scope, Root **proot) {
		Scanner scan(src);
		bool errors = false;
		long long prevAlloc = totalAlloc;
		long long prevObjMem = totalObjMem;
		try {
			Root* root = Parser::parse(&scan, *proot);
			*proot = root;
			new Thread(root, scope);
			while (Thread::current != nullptr) {
				if (Thread::current->step()) {
					// garbage collection if needed
					if (totalAlloc - prevAlloc > threshold || totalObjMem - prevObjMem > threshold) {
						ThreadList::global.mark();
						ObjectList::global.sweep();
						prevAlloc = totalAlloc;
						prevObjMem = totalObjMem;
					}
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
		if (argc < 2) {
			Utils::printErr(L"no input file.\n");
			return 1;
		}
		int ret = 1;
		Root *root = nullptr;
		try {
			Platform::FileReader reader(argv[1]);
			SourceStream source(&reader);
			Environment env;
			env.out = StandartOutputStream::getInstance();
			env.err = StandartErrorStream::getInstance();
			ret = run(&source, &env, BuiltIn::create(&env)->clone(), &root);
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

		while (true) {
			output->write(prompt ? L"? " : L"  ");
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
					run(&source, &env, scope, &root);
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
