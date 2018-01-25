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

#include "Keyword.h"
#include "Resource.h"
#include "Assert.h"

namespace goat {

	Keyword::Keyword(Type _type) : type(_type) {
	}

	Keyword * Keyword::toKeyword() {
		return this;
	}

	String Keyword::toString() {
		switch (type) {
			case VAR: return Resource::s_var;
			case FUNCTION: return Resource::s_function;
			case RETURN: return Resource::s_return;
			case IF: return Resource::s_if;
			case ELSE: return Resource::s_else;
			case FOR: return Resource::s_for;
			case DO: return Resource::s_do;
			case WHILE: return Resource::s_while;
			case NEW: return Resource::s_new;
			case THREAD: return Resource::s_thread;
			case BREAK: return Resource::s_break;
			case CONTINUE: return Resource::s_continue;
			case IN: return Resource::s_in;
			case SWITCH: return Resource::s_switch;
			case CASE: return Resource::s_case;
			case DEFAULT: return Resource::s_default;
			case TRY: return Resource::s_try;
			case CATCH: return Resource::s_catch;
			case FINALLY: return Resource::s_finally;
			case THROW: return Resource::s_throw;
			case IMPORT: return Resource::s_import;
			default:
				assert(false);
				return nullptr;
		}
	}
}