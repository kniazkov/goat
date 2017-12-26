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

namespace goat {

	class Operator : public Token {
	public:
		enum Type {
			PLUS,
			MINUS,
			MUL,
			DIV,
			MOD,
			ASSIGN,
			EQUAL,
			NOT_EQUAL,
			INHERIT,
			LESS,
			GREATER,
			QUESTION,
			LOGICAL_NOT,
			INCREMENT,
			DECREMENT,
			BITWISE_NOT,
			ASSIGN_BY_SUM,
			LEFT_SHIFT,
			SIGNED_RIGHT_SHIFT,
			UNSIGNED_RIGHT_SHIFT,
			LESS_EQUAL,
			GREATER_EQUAL,
			BITWISE_AND,
			BITWISE_XOR,
			BITWISE_OR,
			LOGICAL_AND,
			LOGICAL_OR,
			ASSIGN_BY_DIFF,
			ASSIGN_BY_PRODUCT,
			ASSIGN_BY_QUOTIENT,
			ASSIGN_BY_REMAINDER,
			ASSIGN_BY_LSHIFT,
			ASSIGN_BY_SIGN_RSHIFT,
			ASSIGN_BY_UNSIGN_RSHIFT,
			ASSIGN_BY_AND,
			ASSIGN_BY_XOR,
			ASSIGN_BY_OR,
			UNKNOWN
		};
		String value;
		Type type;

		Operator(String _value);
		Operator *toOperator() override;
	};

}
