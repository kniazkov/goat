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

namespace goat {

	template <typename Type> class Tree {
	protected:
		int height;
		Tree *left, *right;

		Tree(const Tree &) {
		}

		void operator=(const Tree &) {
		}

		int disbalance() {
			return (right ? right->height : 0) - (left ? left->height : 0);
		}

		void updateHeight() {
			int hl = left ? left->height : 0, hr = right ? right->height : 0;
			height = (hl > hr ? hl : hr) + 1;
		}

		Tree * rotateLeft() {
			Tree *t = right;
			this->right = t->left;
			t->left = this;
			updateHeight();
			t->updateHeight();
			return t;
		}

		Tree * rotateRight() {
			Tree *t = left;
			this->left = t->right;
			t->right = this;
			updateHeight();
			t->updateHeight();
			return t;
		}

		Tree * balance() {
			updateHeight();
			if (disbalance() >  1) {
				if (right->disbalance() < 0) {
					right = right->rotateRight();
				}
				return rotateLeft();
			}
			if (disbalance() < -1) {
				if (left->disbalance() > 0) {
					left = left->rotateLeft();
				}
				return rotateRight();
			}
			return this;
		}

		Tree * removeMin() {
			if (!left) {
				return right;
			}
			left = left->removeMin();
			return balance();
		}

	public:
		Type key;

		Tree() : height(1), left(nullptr), right(nullptr) {
		}

		Tree * findMin() {
			return left ? left->findMin() : this;
		}

		Tree * findMax() {
			return right ? right->findMax() : this;
		}

		unsigned int count() {
			return 1 + (left ? left->count() : 0) + (right ? right->count() : 0);
		}

		Tree * getLeft() {
			return left;
		}

		Tree * getRight() {
			return right;
		}

		Tree * insert(Tree *_t) {
			if (_t->key < key) {
				if (left) {
					left = left->insert(_t);
				}
				else {
					left = _t;
				}
			}
			else {
				if (right) {
					right = right->insert(_t);
				}
				else {
					right = _t;
				}
			}
			return balance();
		}

		Tree * find(Type _key) {
			if (key == _key) {
				return this;
			}
			if (_key < key) {
				if (left) {
					return left->find(_key);
				}
			}
			else {
				if (right) {
					return right->find(_key);
				}
			}
			return 0;
		}

		Tree * remove(Type _key, Tree **_removed = nullptr) {
			do {
				if (_key == key) {
					Tree *l = left, *r = right;
					if (_removed) {
						*_removed = this;
						height = 1;
						left = 0;
						right = 0;
					}
					if (!r) {
						return l;
					}
					Tree *q = r->findMin();
					q->right = r->removeMin();
					q->left = l;
					return q->balance();
				}
				else if (_key < key) {
					if (left) {
						left = left->remove(_key, _removed);
					}
					else {
						break;
					}
				}
				else {
					if (right) {
						right = right->remove(_key, _removed);
					}
					else {
						break;
					}
				}
				return balance();
			} while (0);

			if (_removed) {
				*_removed = nullptr;
			}
			return this;
		}

		template <typename Function> void forEach(Function func) {
			if (left) {
				left->forEach(func);
			}
			func(this);
			if (right) {
				right->forEach(func);
			}
		}

	};

}