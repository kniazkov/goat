/*

Copyright (C) 2017-2020 Ivan Kniazkov

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

#pragma once

#include "gc.h"
#include "model/process.h"
#include "lib/ref_counter.h"
#include "model/object_pool.h"
#include "model/context.h"
#include "model/thread.h"
#include "compiler/source/source_manager.h"

namespace g0at
{
    namespace vm
    {
        class environment : public lib::ref_counter
        {
        public:
            environment(gc_type _gc_type, std::vector<std::wstring> &_identifiers_list,
                bool _debug, bool _run, source_manager *_listing, std::vector<char*> *args);
            ~environment();

            gc_type get_gc_type() { return gct; }
            model::object_pool *get_pool() { return pool; }
            model::context *get_context() { return ctx; }
            model::process *get_main_process() { return main_proc; }
            lib::gc *get_gc() { return gc; }
            bool debug_mode() { return debug; }
            bool run_mode() { return run; }
            source_manager *get_listing() { return listing; };

        protected:
            gc_type gct;
            model::object_pool *pool;
            model::context *ctx;
            model::process *main_proc;
            lib::gc *gc;
            bool debug;
            bool run;
            source_manager *listing;

        private:
            environment(const environment &) { }
            void operator=(const environment &) { }
        };
    };
};
