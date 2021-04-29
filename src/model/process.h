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

#include <set>

namespace g0at
{
    namespace model
    {
        class object_pool;
        class thread_list;
        class thread_list_ext;
        class executor;

        class process
        {
        public:
            process(process *_parent, object_pool *_pool);
            ~process();
            const std::set<process*> & get_children() { return children; }

            object_pool *pool;
            executor *exec;
            thread_list_ext *active_threads;
            thread_list *suspended_threads;

        private:
            process *parent;
            std::set<process*> children;

            process(const process&) { }
            void operator=(const process& ) { }
        };
    };
};
