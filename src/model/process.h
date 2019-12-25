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

#pragma once

namespace g0at
{
    namespace model
    {
        class object_pool;
        class thread_list;

        class process
        {
        public:
            process(object_pool *_pool);
            ~process();

            thread_list *get_active_threads() { return active_threads; }
            thread_list *get_suspended_threads() { return suspended_threads; }

        protected:
            object_pool *pool;
            thread_list *active_threads;
            thread_list *suspended_threads;

        private:
            process(const process&) { }
            void operator=(const process& ) { }
        };
    };
};
