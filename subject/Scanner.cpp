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

#include "Scanner.h"
#include "StringBuilder.h"
#include "WideStringBuilder.h"
#include "Identifier.h"
#include "Bracket.h"
#include "StaticString.h"
#include "Semicolon.h"
#include "Operator.h"
#include "Integer.h"
#include "Keyword.h"
#include "Comma.h"
#include "Undefined.h"
#include "Colon.h"
#include "Dot.h"
#include "Boolean.h"
#include "This.h"
#include "Char.h"

namespace goat {

	Scanner::Scanner(Source *_src) {
		src = _src;
	}

	Token * Scanner::getToken(Location **loc) {
		char c = get();

		while (isSpace(c)) {
			c = next();
		}

		*loc = src->location();

		if (!c) {
			return nullptr;
		}
		if (isLetter(c)) {
			StringBuilder b;
			do {
				b << c;
				c = next();
			} while (isLetter(c) || isDigit(c));
			String s = b.toString();
			if (s == "undefined") {
				return new Undefined();
			}
			if (s == "true") {
				return new Boolean(true);
			}
			if (s == "false") {
				return new Boolean(false);
			}
			if (s == "this") {
				return new This();
			}
			if (s == "var") {
				return new Keyword(Keyword::VAR);
			}
			if (s == "function") {
				return new Keyword(Keyword::FUNCTION);
			}
			if (s == "return") {
				return new Keyword(Keyword::RETURN);
			}
			if (s == "if") {
				return new Keyword(Keyword::IF);
			}
			if (s == "else") {
				return new Keyword(Keyword::ELSE);
			}
			if (s == "new") {
				return new Keyword(Keyword::NEW);
			}
			if (s == "while") {
				return new Keyword(Keyword::WHILE);
			}
			if (s == "thread") {
				return new Keyword(Keyword::THREAD);
			}
			Identifier *i = new Identifier();
			i->name = s;
			return i;
		}
		if (isDigit(c)) {
			long long int v = 0;
			do {
				v = v * 10 + c - '0';
				c = next();
			} while (isDigit(c));
			Integer *i = new Integer();
			i->value = v;
			return  i;
		}
		if (isOperator(c)) {
			StringBuilder b;
			do {
				b << c;
				c = next();
			} while (isOperator(c));
			return new Operator(b.toString());
		}
		if (c == '\"') {
			// TODO: errors ???
			c = next();
			WideStringBuilder w;
			while (c != '\"') {
				if (c == '\\') {
					c = next();
					switch (c) {
					case 'n': w << (wchar)'\n'; break;
					case 'r': w << (wchar)'\r'; break;
					case 't': w << (wchar)'\t'; break;
					case '\"': w << (wchar)'\"'; break;
					case '\\': w << (wchar)'\\'; break;
					default : break;
					}
				}
				else
					w << (wchar)c;
				c = next();
			}
			c = next();
			StaticString *ss = new StaticString();
			ss->text = w.toWideString();
			return ss;
		}
		if (c == '\'') {
			// TODO: errors ???
			c = next();
			wchar wc = (wchar)c;
			next();
			next();
			Char * tc = new Char();
			tc->value = wc;
			return tc;
		}
		if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']') {
			Bracket *b = new Bracket();
			b->symbol = c;
			c = next();
			return b;
		}
		if (c == ';') {
			c = next();
			return new Semicolon();
		}
		if (c == ',') {
			c = next();
			return new Comma();
		}
		if (c == ':') {
			c = next();
			return new Colon();
		}
		if (c == '.') {
			c = next();
			return new Dot();
		}
		if (c == '$') {
			c = next();
			return new Keyword(Keyword::FUNCTION);
		}

		throw UnknownSymbol(*loc, c);
	}

	Token * Scanner::getToken() {
		Location *loc;
		Token *tok = getToken(&loc);
		if (tok) {
			tok->loc = loc;
		}
		return tok;
	}

	Scanner::UnknownSymbol::UnknownSymbol(Location *_loc, char _symbol)
		: loc(_loc->toString()), symbol(_symbol) {
	}

	RawString Scanner::UnknownSymbol::toRawString() {
		WideStringBuilder b;
		b << loc << ": unknown symbol " << (wchar)(symbol >= ' ' ? symbol : '?')  << " (" << (int)symbol << ")";
		return b.toRawString();
	}
}
