/*

Copyright (C) 2017-2021 Ivan Kniazkov

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
#include "model/thread.h"
#include "model/process.h"

namespace g0at
{
    namespace vm
    {
        class gc_parallel : public lib::gc
        {
        public:
            gc_parallel(model::runtime *_runtime)
            {
                count = 0;
                stage = 0;
                runtime = _runtime;
                prev_used_memory_size = lib::get_used_memory_size();
            }

            static void mark_all_parallel(model::process *proc)
            {
                model::thread *thr_start = proc->active_threads->get_current_thread();
                model::thread *thr = thr_start;
                if (thr)
                {
                    do
                    {
                         thr->mark_all_parallel();
                         thr = thr->next;
                    } while(thr != thr_start);
                }
                thr_start = proc->suspended_threads->get_current_thread();
                thr = thr_start;
                if (thr)
                {
                    do
                    {
                         thr->mark_all_parallel();
                         thr = thr->next;
                    } while(thr != thr_start);
                }

                const std::set<model::process*> &children_processes = proc->get_children();
                for (model::process *child : children_processes)
                {
                    mark_all_parallel(child);
                }
            }

            void collect_garbage() override
            {
                model::object_pool *pool = runtime->pool;
                if (stage == 0) {
                    count++;
                    stage++;
                    pool->mark_all_static_strings_parallel();
                    mark_all_parallel(runtime->main_proc);
                    return;
                }

                if (stage == 1) {
                    for (int i = 0; i < 10 && pool->gc_deferred.count > 0; i++)
                        pool->gc_deferred.first->mark_parallel(pool);
                    if (pool->gc_deferred.count == 0)
                        stage++;
                    return;
                }

                if (stage == 2) {
                    model::object *obj = pool->population.first;
                    while (obj)
                    {
                        model::object *next = obj->next;
                        obj->sweep(pool);
                        obj = next;
                    }
                    while(pool->gc_processed.count) {
                        obj = pool->gc_processed.remove_first();
                        obj->unmark(false);
                        pool->population.add(obj);
                    }
                    stage = 0;
                    prev_used_memory_size = lib::get_used_memory_size() - lib::get_cached_memory_size();
                }
            }

            void collect_garbage_if_necessary() override
            {
                if ( stage == 0 && lib::get_used_memory_size() - lib::get_cached_memory_size() - prev_used_memory_size < threshold )
                {
                    // keep calm
                    return;
                }

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
            model::runtime *runtime;
            size_t prev_used_memory_size;
            const size_t threshold = 1024 * sizeof(model::context) * 2;
        };

        lib::gc * create_grabage_collector_parallel(model::runtime *runtime)
        {
            return new gc_parallel(runtime);
        }
    };
};
