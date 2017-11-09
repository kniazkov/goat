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

	template <typename Type> class List {
	protected:

		List(const List &) {
		}

		void operator=(const List &) {
		}

	public:
		class Item {
		public:
			Item *prev,
				*next;
			Type data;
		};

		Item *first,
			*last;
		unsigned int count;

		List() : first(nullptr), last(nullptr), count(0) {
		}

		~List() {
			Item *item = first,
				*next;

			while (item) {
				next = item->next;
				delete item;
				item = next;
			}
		}

		void pushBack(Type data) {
			Item *item = new Item();
			
			item->prev = last;
			item->next = nullptr;
			item->data = data;

			if (last) {
				last->next = item;
			}
			else {
				first = item;
			}
			last = item;

			count++;
		}

		void remove(Item *item) {
			if (item->prev) {
				item->prev->next = item->next;
			}
			else {
				first = item->next;
			}

			if (item->next) {
				item->next->prev = item->prev;
			}
			else {
				last = item->prev;
			}

			delete item;
			count--;
		}

		template <typename Function> void forEach(Function func) {
			Item *item = first;
			while (item) {
				Item *next = item->next;
				func(item->data);
				item = next;
			}
		}
	};

}