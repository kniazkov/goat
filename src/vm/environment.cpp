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

#include "environment.h"
#include "model/built_in/context_factory.h"
#include "lib/new.h"

namespace g0at
{
    namespace vm
    {
        environment::environment(gc_type _gc_type, std::vector<std::wstring> &_identifiers_list,
            bool _debug, bool _run, source_manager *_listing)
            : gct(_gc_type), debug(_debug), run(_run), listing(_listing)
        {
            pool = new model::object_pool(_identifiers_list);
            ctx = model::built_in::context_factory(pool).create_context();
            tlist = new model::thread_list(pool);
            proc = new process();
            proc->pool = pool;
            proc->threads = tlist;
            gc = create_garbage_collector(_gc_type, proc);
            lib::set_garbage_collector(gc);
        }

        environment::~environment()
        {
            pool->destroy_all();
            lib::set_garbage_collector(nullptr);
            delete gc;
            delete proc;
            delete tlist;
            delete pool;
        }
    };
};
