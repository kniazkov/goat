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

#include "object_cache.h"
#include "object_pool.h"
#include "object_string.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        object_cache::object_cache()
        {
        }

        void object_cache::init(std::vector<std::wstring> &init_list, object_pool *pool)
        {
            for (int id = 0, size = (int)init_list.size(); id < size; id++)
            {
                std::wstring name = init_list[id];
                assert(indexes.find(name) == indexes.end());
                object_string *obj = pool->create_object_string(name, id);
                indexes.insert(std::pair<std::wstring, int>(name, id));
                objects.push_back(obj);
            }
        }

        object_string *object_cache::get_object(std::wstring name, object_pool *pool)
        {
            auto iter = indexes.find(name);
            if (iter != indexes.end())
            {
                int id = iter->second;
                return objects.at(id);
            }
            else
            {
                int id = (int)objects.size();
                object_string *obj = pool->create_object_string(name, id);
                indexes.insert(std::pair<std::wstring, int>(name, id));
                objects.push_back(obj);
                return obj;
            }
        }

        object_string *object_cache::get_object(int id)
        {
            return objects.at(id);
        }
            
        void object_cache::mark_all()
        {
            for (auto obj: objects)
            {
                obj->mark();
            }
        }
    };
};