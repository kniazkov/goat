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
#include "model/object_pool.h"
#include "model/thread.h"

namespace g0at
{
    namespace vm
    {
        class gc_debug : public lib::gc
        {
        public:
            gc_debug(model::process *_main_proc)
                : count(0), main_proc(_main_proc)
            {
            }

            static void mark_all(model::process *proc)
            {
                model::thread *thr_start = proc->active_threads->get_current_thread();
                model::thread *thr = thr_start;
                if (thr)
                {
                    do
                    {
                         thr->mark_all();
                         thr = thr->next;
                    } while(thr != thr_start);
                }
                thr_start = proc->suspended_threads->get_current_thread();
                thr = thr_start;
                if (thr)
                {
                    do
                    {
                         thr->mark_all();
                         thr = thr->next;
                    } while(thr != thr_start);
                }

                const std::set<model::process*> &children_processes = proc->get_children();
                for (model::process *child : children_processes)
                {
                    mark_all(child);
                }
            }

            void collect_garbage() override
            {
                count++;

                // mark
                main_proc->pool->mark_all_static_strings();
                mark_all(main_proc);

                // sweep
                model::object *obj = main_proc->pool->population.first;
                while (obj)
                {
                    model::object *next = obj->next;
                    obj->sweep(main_proc->pool);
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
            model::process *main_proc;
        };

        lib::gc * create_grabage_collector_debug(model::process *main_proc)
        {
            return new gc_debug(main_proc);
        }
    };
};
