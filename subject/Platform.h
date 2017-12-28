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

#pragma once

#include "Exception.h"
#include "Type.h"
#include "InputStream.h"

namespace goat {

	class Platform {
	public:
		Platform();

		static void * alloc(unsigned int size);
		static void free(void *ptr);
		static void putChar(wchar ch);
		static void putCharErr(wchar ch);
		static wchar getChar();

		class FileNotFound : public Exception {
		protected:
			const char *fname;
		public:
			FileNotFound(const char *_fname);
			RawString toRawString() override;
		};

		class MemoryCorruption : public Exception {
		public:
			RawString toRawString() override;
		};

		class FileReader : public InputStream<char> {
		protected:
			const char *fname;
			void * descriptor;
			int C;
			bool eof;

			FileReader(const FileReader &) {
			}

			void operator=(const FileReader &) {
			}

		public:
			FileReader(const char *_fname);
			~FileReader();
			char read() override;
			bool hasData() override;
			const char *name() override;
		};

		class File {
		protected:
			void * descriptor;

			File(const File &) {
			}

			void operator=(const File &) {
			}

		public:
			enum Mode {
				READ = 0,
				WRITE,
				APPEND
			};

			static File * open(const char *_fname, Mode _mode);
			File() : descriptor(nullptr) {
			}
			~File();
			bool eof();
			int read();
		};
	};

}
