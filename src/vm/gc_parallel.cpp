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
#include "model/context.h"

namespace g0at
{
    namespace vm
    {
        class gc_parallel : public lib::gc
        {
        public:
            gc_parallel(process *_proc)
                : count(0), stage(0), proc(_proc)
            {
            }

            void collect_garbage() override
            {
                if (stage == 0) {
                    count++;
                    proc->pool->mark_all_static_strings_parallel();
                    model::thread *thr_start = proc->threads->get_current_thread();
                    model::thread *thr = thr_start;
                    do
                    {
                        thr->mark_all_parallel();
                        thr = thr->next;
                    } while(thr != thr_start);
                    stage++;
                    return;
                }

                if (stage == 1) {
                    for (int i = 0; i < 10 && proc->pool->gc_deferred.count > 0; i++)
                        proc->pool->gc_deferred.first->mark_parallel(proc->pool);
                    if (proc->pool->gc_deferred.count == 0)
                        stage++;
                    return;
                }

                if (stage == 2) {
                    model::object *obj = proc->pool->population.first;
                    while (obj)
                    {
                        model::object *next = obj->next;
                        obj->sweep(proc->pool);
                        obj = next;
                    }
                    while(proc->pool->gc_processed.count) {
                        obj = proc->pool->gc_processed.remove_first();
                        obj->unmark(false);
                        proc->pool->population.add(obj);
                    }
                    stage = 0;
                }
            }

            void collect_garbage_if_necessary() override
            {
                collect_garbage();
            }

            lib::gc_report get_report() override
            {
                lib::gc_report r;
                r.name = L"parallel";
                r.count_of_launches = count;
                return r;
            }

            int count;
            int stage;
            process *proc;
        };

        lib::gc * create_grabage_collector_parallel(process *proc)
        {
            return new gc_parallel(proc);
        }
    };
};
