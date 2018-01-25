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

#include "Function.h"
#include "Assert.h"
#include "StringBuilder.h"

namespace goat {

	Function::Function() {
		raw = new TokenList();
		args = nullptr;
	}

	Function::Function(Keyword *tokKw, Brackets *tokArgs, Brackets *tokBody) {
		assert(tokArgs->symbol == '(' && tokBody->symbol == '{');
		loc = tokKw->loc;
		args = tokArgs->tokens;
		tokArgs->tokens = nullptr;
		raw = tokBody->tokens;
		tokBody->tokens = nullptr;
	}

	Function::~Function() {
		delete raw;
		delete args;
	}

	Function * Function::toFunction() {
		return this;
	}

	void Function::trace() {
		if (raw) {
			raw->mark();
		}
		if (args) {
			args->mark();
		}
		code.mark();
	}

	State * Function::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * Function::StateImpl::execute() {
		switch (mode)
		{
		case RUN:
			return next();
		case EXCEPTION:
			return throw_(thru);
		case RETURN: {
			State *p = prev;
			prev->ret(thru);
			delete this;
			return p;
		}
		default:
			throw NotImplemented();
		}
	}

	State * Function::StateImpl::next() {
		if (instr) {
			State *st = instr->toStatement()->createState(this);
			instr = instr->next;
			return st;
		}
		else {
			State *p = prev;
			delete this;
			return p;
		}
	}

	void Function::StateImpl::ret(Object *obj) {
		prev->ret(obj);
	}

	String Function::toString() {
		StringBuilder b;
		b << "$(";
		int k = 0;
		if (args) {
			args->forEach([&](Token *tok) {
				if (k) {
					b << ", ";
				}
				k++;
				b << tok->toString();
			});
		}
		b << ") { }";
		return b.toString();
	}

	Token * Function::StateImpl::token() {
		return func;
	}
}
