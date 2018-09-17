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

#pragma once

#include "Tree.h"
#include "Pool.h"
#include "Assert.h"

namespace goat {

	template <typename Key> class Set {
	protected:
		class Item : public Tree<Key> {
		public:

			Item(Key key) {
				this->key = key;
			}

			void destroy() {
				if (this->left) {
					((Item*)this->left)->destroy();
				}
				if (this->right) {
					((Item*)this->right)->destroy();
				}
				delete this;
			}

			void * operator new(__size size) {
				return Pool<sizeof(Item)>::getInstance().alloc(size);
			}

			void operator delete(void *ptr) {
				Pool<sizeof(Item)>::getInstance().free(ptr);
			}

			Item *clone() {
				Item *i = new Item(this->key);
				i->height = this->height;
				if (this->left) {
					i->left = ((Item*)this->left)->clone();
				}
				if (this->right) {
					i->right = ((Item*)this->right)->clone();
				}
				return i;
			}
		};

		Item *top;

		Set(const Set &) {
		}

		void operator=(const Set &) {
		}

	public:
		Set() : top(nullptr) {
		}

		~Set() {
			if (top) {
				top->destroy();
			}
		}

		void insert(Key key) {
			if (top) {
				Item *item = (Item*)top->find(key);
				if (!item) {
					top = (Item*)top->insert(new Item(key));
				}
			}
			else {
				top = new Item(key);
			}
		}

		bool contains(Key key) {
			if (top) {
				Item *item = (Item*)top->find(key);
				if (item) {
					return true;
				}
			}
			return false;
		}

		void remove(Key key) {
			if (top) {
				Item *removed;
				top = (Item*)top->remove(key, (Tree<Key>**)(&removed));
				if (removed) {
					delete removed;
				}
			}
		}

		template <typename Function> void forEach(Function func) {
			if (top) {
				top->forEach([&](Tree<Key> *node) {
					Item* item = (Item*)node;
					func(item->key);
				});
			}
		}

		void clone(Set &set) {
			assert(set.top == nullptr);
			if (top) {
				set.top = top->clone();
			}
		}
	};

}
