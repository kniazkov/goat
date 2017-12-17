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
#include "Unicode.h"
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
#include "Real.h"

namespace goat {

	Scanner::Scanner(Source *_src) {
		src = _src;
	}

	Token * Scanner::getToken(Location **loc) {
		char c = get();

		while (isSpace(c)) {
			c = next();
		}

		while (true) {
			if (c == '/') {
				c = next();
				if (c == '*') {
					c = next();
					while (true) {
						if (!c) {
							throw UnexpectedEnd(src->location());
						}
						if (c == '*') {
							c = next();
							if (c == '/') {
								c = next();
								while (isSpace(c)) {
									c = next();
								}
								break;
							}
						}
						else {
							c = next();
						}
					}
				}
				else if (c == '/') {
					c = next();
					while (c != '\0' && c != '\n' && c != '\r') {
						c = next();
					}
					while (isSpace(c)) {
						c = next();
					}
				}
				else {
					unget(c);
					unget('/');
					c = '/';
					break;
				}
			}
			else {
				break;
			}
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
			if (s == "for") {
				return new Keyword(Keyword::FOR);
			}
			if (s == "do") {
				return new Keyword(Keyword::DO);
			}
			if (s == "while") {
				return new Keyword(Keyword::WHILE);
			}
			if (s == "thread") {
				return new Keyword(Keyword::THREAD);
			}
			if (s == "break") {
				return new Keyword(Keyword::BREAK);
			}
			if (s == "continue") {
				return new Keyword(Keyword::CONTINUE);
			}
			if (s == "in") {
				return new Keyword(Keyword::IN);
			}
			if (s == "switch") {
				return new Keyword(Keyword::SWITCH);
			}
			if (s == "case") {
				return new Keyword(Keyword::CASE);
			}
			if (s == "default") {
				return new Keyword(Keyword::DEFAULT);
			}
			if (s == "try") {
				return new Keyword(Keyword::TRY);
			}
			if (s == "catch") {
				return new Keyword(Keyword::CATCH);
			}
			if (s == "finally") {
				return new Keyword(Keyword::FINALLY);
			}
			if (s == "throw") {
				return new Keyword(Keyword::THROW);
			}
			Identifier *i = new Identifier();
			i->name = s;
			return i;
		}
		if (isDigit(c)) {
			long long int iv = 0;
			do {
				iv = iv * 10 + c - '0';
				c = next();
			} while (isDigit(c));
			if (c != '.') {
				Integer *i = new Integer();
				i->value = iv;
				return  i;
			}
			// c == '.' and then digit => real value
			c = next();
			if (!isDigit(c)) {
				unget(c);
				unget('.');
				Integer *i = new Integer();
				i->value = iv;
				return  i;
			}
			long long int fv = 0,
				m = 1;
			do {
				m = m * 10;
				fv = fv * 10 + c - '0';
				c = next();
			} while (isDigit(c));
			Real *r = new Real();
			r->value = (long double)iv + (long double)fv / (long double)m;
			return  r;
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
			WideString w = parseString('\"');
			StaticString *s = new StaticString();
			s->text = w;
			return s;
		}
		if (c == '\'') {
			WideString w = parseString('\'');
			Char * tc = new Char();
			if (w.len() != 1) {
				throw IncorrectCharConst(src->location());
			}
			tc->value = w[0];
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

	WideString Scanner::parseString(char separator) {
		char c = next();
		StringBuilder s;
		while (c != separator) {
			if (c == '\\') {
				c = next();
				switch (c) {
				case 'n': s << '\n'; break;
				case 'r': s << '\r'; break;
				case 't': s << '\t'; break;
				case '\"': s << '\"'; break;
				case '\'': s << '\''; break;
				case '\\': s << '\\'; break;
				default : throw IncorrectSequence(src->location(), c);
				}
			}
			else if (c == '\0') {
				throw UnexpectedEnd(src->location());
			}
			else
				s << c;
			c = next();
		}
		c = next();
		return Unicode::UTF8Decode(s.toString());
	}

	Token * Scanner::getToken() {
		Location *loc;
		Token *tok = getToken(&loc);
		if (tok) {
			tok->loc = loc;
		}
		return tok;
	}

	RawString Scanner::ScanError::toRawString() {
		return (WideStringBuilder() << loc << ", scan error: " << message()).toRawString();
	}

	WideString Scanner::UnknownSymbol::message() {
		return (WideStringBuilder() << "unknown symbol " << (wchar)(symbol >= ' ' ? symbol : '?') << " (" << (int)symbol << ")").toWideString();
	}

	WideString Scanner::IncorrectSequence::message() {
		return (WideStringBuilder() << "incorrect char sequence \\" << (wchar)(symbol >= ' ' ? symbol : '?') << " (" << (int)symbol << ")").toWideString();
	}

	WideString Scanner::UnexpectedEnd::message() {
		return L"unexpected end of file";
	}

	WideString Scanner::IncorrectCharConst::message() {
		return L"incorrect character constant";
	}
}
