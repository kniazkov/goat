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

#include <string>
#include <map>
#include <vector>

namespace g0at
{
    namespace model
    {
        class object_pool;
        class object_string;
        
        class object_cache
        {
        public:
            object_cache();
            void merge(std::vector<std::wstring> &list, object_pool *pool);
            object_string *get_object(std::wstring name, object_pool *pool);
            object_string *get_object(int id);
            std::vector<std::wstring> get_strings_list();
            void mark_all();

            int get_size() { return (int)objects.size(); }

        protected:
            object_cache(const object_cache&) { }
            void operator=(const object_cache&) { }

            std::map<std::wstring, int> indexes;
            std::vector<object_string *> objects;
        };
    };
};