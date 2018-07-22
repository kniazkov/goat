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

#include "Try.h"
#include "Assert.h"
#include "StringBuilder.h"

namespace goat {

	Try::Try(Keyword *_kw, Statement *_stmtTry, Identifier *_varName, Statement *_stmtCatch, Statement *_stmtFinally) {
		loc = _kw->loc;
		stmtTry = _stmtTry;
		varName = _varName;
		stmtCatch = _stmtCatch;
		stmtFinally = _stmtFinally;
		assert(stmtCatch != nullptr || stmtFinally != nullptr);
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

	State * Try::StateImpl::execute() {
		switch (mode)
		{
			case RUN:
				return next();
			case EXCEPTION:
				if (!catched) {
					catched = true;
					mode = RUN;
					step = stmt->stmtCatch ? CATCH : FINALLY;
					return next();
				}
				return throw_(thru);
			case RETURN:
				if (step == FINALLY) {
					return next();
				}
				return return_(thru);
			case BREAK:
				return break_();
			case CONTINUE:
				return continue_();
			default:
				throw NotImplemented();
		}
	}

	State * Try::StateImpl::next() {
		switch (step) {
			case TRY:
				step = stmt->stmtFinally ? FINALLY : DONE;
				return stmt->stmtTry->createState(this);
			case CATCH:
				step = stmt->stmtFinally ? FINALLY : DONE;
				cloneScope();
				scope->objects.insert(Object::createIndex(stmt->varName->name), thru);
				return stmt->stmtCatch->createState(this);
			case FINALLY:
				step = DONE;
				return stmt->stmtFinally->createState(this);
			case DONE: {
				State *p = prev;
				delete this;
				return p;
			}
			default:
				throw NotImplemented();
		}
	}

	void Try::StateImpl::ret(Object *obj) {

	}

	void Try::StateImpl::trace() {

	}

	String Try::toString() {
		StringBuilder b;
		b << "try " << stmtTry->toString();
		if (stmtCatch) {
			b << " catch (" << varName->name << ") " << stmtCatch->toString();
		}
		if (stmtFinally) {
			b << " finally " << stmtFinally->toString();
		}
		return b.toString();
	}

	Token * Try::StateImpl::token() {
		return stmt;
	}
}
