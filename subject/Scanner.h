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

#include "Token.h"
#include "Source.h"
#include "Exception.h"
#include "Assert.h"

namespace goat {

	class Scanner {
	protected:
		Source *src;
		char c1;

		inline char get();
		inline void unget(char c);
		inline char next();
		static inline bool isSpace(char c);
		static inline bool isLetter(char c);
		static inline bool isDigit(char c);
		static inline bool isOperator(char c);
		Token * getToken(Location **loc);

	public:
		Scanner(Source *_src);
		Token * getToken();

		class ScanError : public Exception {
		public:
			String loc;

			ScanError(Location* _loc) : loc(_loc->toString()) {
			}

			RawString toRawString() override;
			virtual WideString message() = 0;
		};

		class UnknownSymbol : public ScanError {
		public:
			char symbol;

			UnknownSymbol(Location *_loc, char _symbol) : ScanError(_loc), symbol(_symbol) {
			};
			WideString message() override;
		};

		class UnexpectedEnd : public ScanError {
		public:
			UnexpectedEnd(Location *_loc) : ScanError(_loc) {
			};
			WideString message() override;
		};
	};

	char Scanner::get() {
		return c1 != '\0' ? c1 : src->get();
	}

	void Scanner::unget(char c) {
		assert(c1 == '\0');
		c1 = c;
	}

	char Scanner::next() {
		if (c1 != '\0') {
			char c = c1;
			c1 = '\0';
			return c;
		}
		else
			return src->next();
	}

	bool Scanner::isSpace(char c) {
		switch (c)
		{
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return true;
		default:
			return false;
		}
	}

	bool Scanner::isLetter(char c) {
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
	}

	bool Scanner::isDigit(char c) {
		return c >= '0' && c <= '9';
	}

	bool Scanner::isOperator(char c) {
		switch (c)
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '=':
		case '!':
		case '>':
		case '<':
			return true;
		default:
			return false;
		}
	}
}
