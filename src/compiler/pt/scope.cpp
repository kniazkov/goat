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

#include "scope.h"
#include <sstream>

namespace g0at
{
    namespace pt
    {
        symbol::symbol(int _id, std::wstring _name)
            : id(_id), name(_name), last_type(nullptr)
        {
        }

        type::type()
        {
        }

        scope::scope()
        {
        }

        scope::scope(lib::pointer<scope> &_parent)
        {
            parents.push_back(_parent.get());
        }

        std::wstring scope::get_symbols_list()
        {
            std::wstringstream wss;
            dictionary tmp;
            flat(tmp);
            bool first = true;
            for (auto pair : tmp)
            {
                if (!first)
                    wss << L", ";
                else
                    first = false;
                wss << pair.first << L'(' << pair.second->get_id() << L')';
            }
            return wss.str();
        }

        void scope::flat(dictionary &dst)
        {
            for (int i = (int)parents.size() - 1; i > -1; i--)
            {
                parents[i]->flat(dst);
            }

            for(auto pair : symbols)
            {
                dst[pair.first] = pair.second;
            }
        }
    };
};
