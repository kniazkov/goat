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
#include "lib/new.h"

namespace g0at
{
    namespace vm
    {
        class gc_serial : public lib::gc
        {
        public:
            gc_serial(process *_proc)
            {
                count = 0;
                proc = _proc;
                prev_used_memory_size = lib::get_used_memory_size();
            }

            void collect_garbage() override
            {
                count++;

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

                prev_used_memory_size = lib::get_used_memory_size();
            }

            void collect_garbage_if_necessary() override
            {
                if ( lib::get_used_memory_size() - prev_used_memory_size < threshold )
                {
                    // keep calm
                    return;
                }

                collect_garbage();
            }

            lib::gc_report get_report() override
            {
                lib::gc_report r;
                r.name = L"serial";
                r.count_of_launches = count;
                return r;
            }

            int count;
            process *proc;
            size_t prev_used_memory_size;
            const size_t threshold = 1 * 1024 * 1024;
        };

        lib::pointer<lib::gc> create_grabage_collector_serial(process *proc)
        {
            return new gc_serial(proc);
        }
    };
};
