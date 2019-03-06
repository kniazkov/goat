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
        class gc_serial : public gc
        {
        protected:
            gc_serial()
            {
                count = 0;
                prev_used_memory_size = lib::get_used_memory_size();
            }

        public:
            static gc * get_instance()
            {
                static gc_serial instance;
                return &instance;
            }


            void collect_garbage(process *proc)
            {
                if ( lib::get_used_memory_size() - prev_used_memory_size < threshold )
                {
                    // keep calm
                    return;
                }

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

            const wchar_t *get_name() override
            {
                return L"serial";
            }

            int get_count_of_launches() override
            {
                return count;
            }

            int count;
            size_t prev_used_memory_size;
            const size_t threshold = 1 * 1024 * 1024;
        };

        gc * gc::get_instance_serial()
        {
            return gc_serial::get_instance();
        }
    };
};
