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
			type = NOT;
		}
		else if (value == "++") {
			type = INCREMENT;
		}
		else if (value == "--") {
			type = DECREMENT;
		}
		else {
			type = UNKNOWN;
		}
	}

	Operator * Operator::toOperator() {
		return this;
	}

}
