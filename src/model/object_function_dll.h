/*

Copyright (C) 2021 Ivan Kniazkov

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

#include "object_function.h"

namespace g0at
{
    namespace model
    {
        /**
         * @brief Wrapper for calling a function that implemented on C 
         *        and dynamically loaded from DLL file
         */

        struct ext_thread_runner_data
        {
            object_pool *pool;
            process *proc;

            ext_thread_runner_data(object_pool *_pool, process *_proc) : pool(_pool), proc(_proc) { }
        };  
            
        class object_function_dll : public object_function
        {
        public:
            object_function_dll(object_pool *_pool, object_dynamic_library *_library, goat_ext_function _ext_func);
            ~object_function_dll();
            void call(thread *thr, int arg_count, call_mode mode) override;
            void trace() override;
            void trace_parallel(object_pool *pool) override;
        
        private:
            object_dynamic_library *library;
            goat_ext_function ext_func;

            goat_shell *shell;
            goat_thread_runner *runner;
            ext_thread_runner_data *runner_data;
        };
    };
};
