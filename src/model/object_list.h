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

#pragma once

namespace g0at
{
    namespace model
    {
        class object;

        class object_list
        {
        public:
            object_list();
            void swap(object_list &other); 
            void add(object *item);
            void remove(object *item);
            object *remove();
            object *remove_first();
            void destroy_all();
            bool contains(object *item);

            object *first;
            object *last;
            int count;

        protected:
            object_list(const object_list &) { }
            void operator=(const object_list &) { }
        };
    };
};
