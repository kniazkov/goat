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
#include "vm/vm_exception.h"

namespace g0at
{
    namespace model
    {
        class stack
        {
        public:
            stack()
                : top(nullptr), used_count(0), pool(nullptr), free_count(0)
            {
            }

            ~stack()
            {
                item *it = top;
                while(it)
                {
                    item *next = it->next;
                    delete it;
                    it = next;
                }
                it = pool;
                while(it)
                {
                    item *next = it->next;
                    delete it;
                    it = next;
                }
            }

            variable *push(variable &var)
            {
                item *it;
                if (pool != nullptr)
                {
                    it = pool;
                    pool = pool->next;
                    free_count--;
                }
                else
                {
                    it = new item();
                }
                it->var = var;
                it->next = top;
                top = it;
                used_count++;
                return &it->var;
            }

            variable pop()
            {
                if (!top)
                    throw vm::stack_is_empty();
                item *it = top;
                variable var = top->var;
                top = top->next;
                delete_or_cache_item(it);
                used_count--;
                return var;
            }

            void pop(int n)
            {
                while (n > 0)
                {
                    if (!top)
                        throw vm::stack_is_empty();
                    item *it = top;
                    top = top->next;
                    delete_or_cache_item(it);
                    used_count--;
                    n--;
                }
            }

            void restore_size(int size)
            {
                if (size < 0)
                    return;

                while(used_count > size)
                {
                    item *it = top;
                    top = top->next;
                    delete_or_cache_item(it);
                    used_count--;
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

            int size()
            {
                return used_count;
            }

            void mark_all()
            {
                item *it = top;
                while(it)
                {
                    it->var.mark();
                    it = it->next;
                }
            }

            void mark_all_parallel(object_pool *pool)
            {
                item *it = top;
                while(it)
                {
                    it->var.mark_parallel(pool);
                    it = it->next;
                }
            }

        protected:
            struct item
            {
                variable var;
                item *next;
            };

            stack(const stack &) { }
            void operator=(const stack &) { }
            
            bool item_should_be_deleted()
            {
                return free_count > free_count_min && free_count > used_count * factor;
            }

            void delete_or_cache_item(item *it)
            {
                if (item_should_be_deleted())
                {
                    delete it;
                }
                else
                {
                    it->next = pool;
                    pool = it;
                    free_count++;
                }
            }

            item *top;
            int used_count;
            item *pool;
            int free_count;
            static const int free_count_min = 1024;
            static const int factor = 2;
        };
    };
};
