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

#define _CRT_SECURE_NO_WARNINGS

#include "Platform.h"
#include "WideStringBuilder.h"
#include "Utils.h"
#include "Unicode.h"
#include "FileName.h"

#include <cstdio>
#include <cstdlib>
#ifdef _WIN32
	#include <io.h>
	#include <fcntl.h>
#endif
#include <chrono>

namespace goat {

	Platform::Platform() {
#ifdef _WIN32
		_setmode(_fileno(stdout), O_BINARY);
		_setmode(_fileno(stdin), O_BINARY);
		_setmode(_fileno(stderr), O_BINARY);
#endif
	}

	Platform __plaform;

#if 0
	void * Platform::alloc(unsigned int size) {
		return std::malloc(size);
	}

	void Platform::free(void *ptr) {
		return std::free(ptr);
	}
#else
	void * Platform::alloc(unsigned int size) {
		unsigned int dbgSize = size + sizeof(unsigned int) + 16;
		char *mem = (char*)std::malloc(dbgSize);
		if (mem) {
			unsigned int *pSize = (unsigned int*)mem;
			char *buff = mem + sizeof(unsigned int);
			char *dbgBuff = buff + size;
			unsigned int i;

			*pSize = size;
			for (i = 0; i < 16; i++) {
				dbgBuff[i] = 0xA;
			}

			return (void*)buff;
		}
		else return nullptr;
	}

	void Platform::free(void *ptr) {
		if (ptr) {
			char *buff = (char*)ptr;
			unsigned int *pSize = (unsigned int*)(buff - sizeof(unsigned int));
			char *dbgBuff = buff + *pSize;
			unsigned int i;
			for (i = 0; i < 16; i++) {
				if (dbgBuff[i] != 0xA) {
					throw MemoryCorruption();
				}
			}

			std::free(pSize);
		}
	}
#endif

	void Platform::putChar(wchar w) {
#if 0
		std::putchar(w < 128 ? w : '?');
#else
		char c[4];
		unsigned int i,
			n = Unicode::UTF8EncodeChar(w, c);
		for (i = 0; i < n; i++) {
			std::putchar(c[i]);
		}
#endif
	}

	void Platform::putCharErr(wchar ch) {
		std::putc(ch < 128 ? ch : '?', stderr);
	}

	wchar Platform::getChar() {
		int c = std::getchar();
		if (c == EOF) {
			return 0;
		}
		return (wchar)c;
	}

	lint Platform::getTimeNs()
	{
		auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
		auto tn = std::chrono::duration_cast<std::chrono::nanoseconds>(t);
		return tn.count();
	}
	
	
	
	Platform::FileNotFound::FileNotFound(String _fname) : fname(_fname/*FileName::extractName(_fname)*/) {
	}

	RawString Platform::FileNotFound::toRawString() {
		return (WideStringBuilder() << "file not found: \'" << fname << (wchar)'\'').toRawString();
	}

	RawString Platform::MemoryCorruption::toRawString() {
		return L"memory corruption";
	}

	
	
	Platform::FileReader::FileReader(String _fname) {
		fname = _fname;
		FILE *stream = std::fopen(fname.cstr(), "rb");
		if (!stream) {
			throw Platform::FileNotFound(fname);
		}
		C = std::fgetc(stream);
		eof = (C == EOF);
		descriptor = (void*)stream;
	}

	Platform::FileReader::~FileReader() {
		if (descriptor) {
			std::fclose((FILE*)descriptor);
		}
	}

	char Platform::FileReader::read() {
		if (eof) {
			throw NoData();
		}
		char ret = (char)C;
		C = fgetc((FILE*)descriptor);
		eof = (C == EOF);
		return ret;
	}

	bool Platform::FileReader::hasData() {
		return eof == false;
	}

	String Platform::FileReader::name() {
		return FileName::extractName(fname);
	}

	bool Platform::fileExist(String fname) {
		FILE *stream = std::fopen(fname.cstr(), "rb");
		if (stream) {
			std::fclose(stream);
			return true;
		}
		return false;
	}

	Platform::File * Platform::File::open(String fname, Mode mode) {
		if (fname.len() == 0) {
			return nullptr;
		}
		fname = FileName::normalize(fname);
		FILE *stream = nullptr;
		switch (mode) {
			case READ:
				stream = std::fopen(fname.cstr(), "r");
				break;
			case WRITE:
				stream = std::fopen(fname.cstr(), "w");
				break;
			case APPEND:
				stream = std::fopen(fname.cstr(), "a");
				break;
			default:
				break;
		}
		if (!stream) {
			return nullptr;
		}

		File *file = new File();
		file->descriptor = (void*)stream;
		return file;
	}

	Platform::File::~File() {
		std::fclose((FILE*)descriptor);
	}

	bool Platform::File::eof() {
		return std::feof((FILE*)descriptor) != 0;
	}

	int Platform::File::read() {
		int ch = std::fgetc((FILE*)descriptor);
		return ch != EOF ? (char)ch : -1;
	}

	long int Platform::File::read(unsigned char *buff, long int count) {
		return (long int)std::fread(buff, 1, (size_t)count, (FILE*)descriptor);
	}

	long int Platform::File::position() {
		return std::ftell((FILE*)descriptor);
	}

	bool Platform::File::seek(long int offset, Origin origin) {
		switch (origin) {
			case BEGIN:
				return fseek((FILE*)descriptor, offset, SEEK_SET) == 0;
			case END:
				return fseek((FILE*)descriptor, offset, SEEK_END) == 0;
			case CURRENT:
				return fseek((FILE*)descriptor, offset, SEEK_CUR) == 0;
		}
		return false;
	}
}
