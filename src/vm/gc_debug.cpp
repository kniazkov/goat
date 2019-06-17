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
        class gc_debug : public lib::gc
        {
        public:
            gc_debug(process *_proc)
                : count(0), proc(_proc)
            {    
            }

            void collect_garbage() override
            {
                count++;

                // mark
                proc->pool->mark_all_static_strings();
                model::thread *thr_start = proc->threads->get_current_thread();
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

            void collect_garbage_if_necessary() override
            {
                collect_garbage();
            }

            lib::gc_report get_report() override
            {
                lib::gc_report r;
                r.name = L"debug";
                r.count_of_launches = count;
                return r;
            }

            int count;
            process *proc;
        };

        lib::pointer<lib::gc> create_grabage_collector_debug(process *proc)
        {
            return new gc_debug(proc);
        }
    };
};
