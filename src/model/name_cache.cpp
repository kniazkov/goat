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

#include "name_cache.h"

namespace g0at
{
    namespace model
    {
        name_cache::name_cache()
        {
        }

        void name_cache::reinit(std::vector<std::wstring> new_vector)
        {
            map.clear();
            vector = new_vector;

            for (int id = 0, size = (int)new_vector.size(); id < size; id++)
            {
                map.insert(std::pair<std::wstring, int>(new_vector[id], id));
            }
        }

        int name_cache::get_id(std::wstring name)
        {
            auto iter = map.find(name);
            if (iter != map.end())
                return iter->second;

            int id = (int)vector.size();
            map.insert(std::pair<std::wstring, int>(name, id));
            vector.push_back(name);
            return id;
        }
    };
};