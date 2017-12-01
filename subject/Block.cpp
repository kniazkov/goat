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

#include "Block.h"
#include "Assert.h"

namespace goat {

	Block::Block(Brackets *brackets) {
		assert(brackets->symbol == '{');

		loc = brackets->loc;
		tokens = brackets->tokens;
		brackets->tokens = nullptr;
	}

	Block::~Block() {
		delete tokens;
	}

	void Block::trace() {
		tokens->mark();
	}

	Block * Block::toBlock() {
		return this;
	}

	State * Block::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	Block::StateImpl::StateImpl(State *_prev, Block *_block) : State(_prev), block(_block), tok(_block->tokens->first) {
		cloneScope();
	}

	State * Block::StateImpl::next() {
		if (tok) {
			State *st = tok->toStatement()->createState(this);
			tok = tok->next;
			return st;
		}
		else {
			State *p = prev;
			delete this;
			return p;
		}
	}

}
