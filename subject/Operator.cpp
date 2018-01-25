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

#include "Operator.h"

namespace goat {

	Operator::Operator(String _value) {
		value = _value;
		if (value == "+") {
			type = PLUS;
		}
		else if (value == "-") {
			type = MINUS;
		}
		else if (value == "*") {
			type = MUL;
		}
		else if (value == "/") {
			type = DIV;
		}
		else if (value == "%") {
			type = MOD;
		}
		else if (value == "=") {
			type = ASSIGN;
		}
		else if (value == "==") {
			type = EQUAL;
		}
		else if (value == "!=") {
			type = NOT_EQUAL;
		}
		else if (value == "->") {
			type = INHERIT;
		}
		else if (value == "<") {
			type = LESS;
		}
		else if (value == ">") {
			type = GREATER;
		}
		else if (value == "?") {
			type = QUESTION;
		}
		else if (value == "!") {
			type = LOGICAL_NOT;
		}
		else if (value == "++") {
			type = INCREMENT;
		}
		else if (value == "--") {
			type = DECREMENT;
		}
		else if (value == "~") {
			type = BITWISE_NOT;
		}
		else if (value == "+=") {
			type = ASSIGN_BY_SUM;
		}
		else if (value == "<<") {
			type = LEFT_SHIFT;
		}
		else if (value == ">>") {
			type = SIGNED_RIGHT_SHIFT;
		}
		else if (value == ">>>") {
			type = UNSIGNED_RIGHT_SHIFT;
		}
		else if (value == "<=") {
			type = LESS_EQUAL;
		}
		else if (value == ">=") {
			type = GREATER_EQUAL;
		}
		else if (value == "|") {
			type = BITWISE_OR;
		}
		else if (value == "&") {
			type = BITWISE_AND;
		}
		else if (value == "^") {
			type = BITWISE_XOR;
		}
		else if (value == "||") {
			type = LOGICAL_OR;
		}
		else if (value == "&&") {
			type = LOGICAL_AND;
		}
		else if (value == "-=") {
			type = ASSIGN_BY_DIFF;
		}
		else if (value == "*=") {
			type = ASSIGN_BY_PRODUCT;
		}
		else if (value == "/=") {
			type = ASSIGN_BY_QUOTIENT;
		}
		else if (value == "%=") {
			type = ASSIGN_BY_REMAINDER;
		}
		else if (value == "<<=") {
			type = ASSIGN_BY_LSHIFT;
		}
		else if (value == ">>=") {
			type = ASSIGN_BY_SIGN_RSHIFT;
		}
		else if (value == ">>>=") {
			type = ASSIGN_BY_UNSIGN_RSHIFT;
		}
		else if (value == "&=") {
			type = ASSIGN_BY_SUM;
		}
		else if (value == "^=") {
			type = ASSIGN_BY_XOR;
		}
		else if (value == "|=") {
			type = ASSIGN_BY_OR;
		}
		else {
			type = UNKNOWN;
		}
	}

	Operator * Operator::toOperator() {
		return this;
	}

	String Operator::toString() {
		return value;
	}
}
