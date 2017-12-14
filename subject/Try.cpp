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

#include "Try.h"

namespace goat {

	Try::Try(Keyword *_kw, Statement *_stmtTry, Identifier *_varName, Statement *_stmtCatch, Statement *_stmtFinally) {
		loc = _kw->loc;
		stmtTry = _stmtTry;
		varName = _varName;
		stmtCatch = _stmtCatch;
		stmtFinally = _stmtFinally;
	}

	void Try::trace() {
		stmtTry->mark();
		if (varName) {
			varName->mark();
		}
		if (stmtCatch) {
			stmtCatch->mark();
		}
		if (stmtFinally) {
			stmtFinally->mark();
		}
	}

	Try * Try::toTry() {
		return this;
	}

	State * Try::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * Try::StateImpl::next() {
		return nullptr;
	}

	void Try::StateImpl::ret(Object *obj) {

	}

	void Try::StateImpl::trace() {

	}
}
