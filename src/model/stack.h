/*

Copyright (C) 17..19 Ivan Kniazkov

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

#include "object.h"

namespace g0at
{
    namespace model
    {
        class stack
        {
        public:
            stack()
                : top(nullptr)
            {
            }

            ~stack()
            {
                item *it = top;
                while(it)
                {
                    item *next = top->next;
                    delete it;
                    it = next;
                }
            }

            variable *push(variable &var)
            {
                item *it = new item();
                it->var = var;
                it->next = top;
                top = it;
                return &it->var;
            }

            variable pop()
            {
                item *it = top;
                variable var = top->var;
                top = top->next;
                delete it;
                return var;              
            }

            void pop(int n)
            {
                while (n > 0)
                {
                    item *it = top;
                    top = top->next;
                    delete it;
                    n--;
                }
            }

            variable &peek()
            {
                return top->var;
            }

            variable &peek(int n)
            {
                item *it = top;
                while(n > 0)
                {
                    it = it->next;
                    n--;
                }
                return it->var;
            }

            bool empty()
            {
                return top == nullptr;
            }

        protected:
            struct item
            {
                variable var;
                item *next;
            };

            stack(const stack &) { }
            void operator=(const stack &) { }

            item *top;
        };
    };
};