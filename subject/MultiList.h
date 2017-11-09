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

#include "Exception.h"

namespace goat {

	class WrongList : public Exception {
	public:
		RawString toRawString() override;
	};

	template <typename Type, Type* Type::*Prev, Type* Type::*Next, void* Type::*List> class MultiList {
	public:
		Type *first,
			*last;
		unsigned int count;

		MultiList() : first(nullptr), last(nullptr), count(0) {
		}

		void pushBack(Type *item) {
			if (item->*List) {
				((decltype(this))(item->*List))->remove(item);
			}

			item->*Prev = last;
			item->*Next = nullptr;
			item->*List = this;

			if (last) {
				last->*Next = item;
			}
			else {
				first = item;
			}
			last = item;

			count++;
		}

		void pushAfter(Type *item, Type *after) {
			if (item->*List) {
				((decltype(this))(item->*List))->remove(item);
			}

			item->*Prev = after;
			item->*Next = after->*Next;
			item->*List = this;

			if (!(after->*Next)) {
				last = item;
			}
			else {
				after->*Next->*Prev = item;
			}
			after->*Next = item;

			count++;
		}

		void remove(Type *item) {
			if (item->*List != this) {
				throw WrongList();
			}

			if (item->*Prev) {
				item->*Prev->*Next = item->*Next;
			}
			else {
				first = item->*Next;
			}

			if (item->*Next) {
				item->*Next->*Prev = item->*Prev;
			}
			else {
				last = item->*Prev;
			}

			item->*List = nullptr;
			item->*Prev = nullptr;
			item->*Next = nullptr;

			count--;
		}

		void replace(Type *begin, Type *end, Type *repl) {
			if (begin->*List != this || end->*List != this) {
				throw WrongList();
			}

			if (repl->*List) {
				((decltype(this))(repl->*List))->remove(repl);
			}
			repl->*List = this;

			if (begin->*Prev) {
				begin->*Prev->*Next = repl;
				repl->*Prev = begin->*Prev;
			}
			else {
				first = repl;
				repl->*Prev = nullptr;
			}

			if (end->*Next) {
				end->*Next->*Prev = repl;
				repl->*Next = end->*Next;
			}
			else {
				last = repl;
				repl->*Next = nullptr;
			}

			end = end->*Next;
			while (begin != end) {
				Type *next = begin->*Next;
				begin->*List = nullptr;
				begin->*Prev = nullptr;
				begin->*Next = nullptr;
				begin = next;
				count--;
			}

			count++;
		}

		template <typename Function> void forEach(Function func) {
			Type *item = first;
			while (item) {
				Type *next = item->*Next;
				func(item);
				item = next;
			}
		}

		template <typename Function> void forEachRev(Function func) {
			Type *item = last;
			while (item) {
				Type *prev = item->*Prev;
				func(item);
				item = prev;
			}
		}
	};

}