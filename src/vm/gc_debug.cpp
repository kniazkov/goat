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

#include "gc.h"

namespace g0at
{
    namespace vm
    {
        class gc_debug : public gc
        {
        protected:
            gc_debug() { }

        public:
            static gc * get_instance()
            {
                static gc_debug instance;
                return &instance;
            }

            void collect_garbage(process *proc)
            {
                // mark
                proc->cache->mark_all();
                model::thread *thr_start = proc->threads;
                model::thread *thr = thr_start;
                do
                {
                    thr->mark_all();
                    thr = thr->next;
                } while(thr != thr_start);

                // sweep
                model::object *obj = proc->pool->population.first;
                while (obj)
                {
                    model::object *next = obj->next;
                    obj->sweep(proc->pool);
                    obj = next;
                }
            }
        };

        gc * gc::get_instance_debug()
        {
            return gc_debug::get_instance();
        }
    };
};
