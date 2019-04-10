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

        symbol::symbol(const wchar_t *_name, type *_type)
            : id(-1), name(_name), last_type(_type)
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
            parents.push_back(_parent);
        }

        std::vector<scope::descriptor> scope::get_symbol_table()
        {
            std::vector<descriptor> result;
            // prepare flat array
            dictionary inherited;
            for (int i = (int)parents.size() - 1; i > -1; i--)
            {
                parents[i]->flat(inherited);
            }
            // own symbols
            for(auto pair : symbols)
            {
                descriptor descr = {0};
                descr.sl = pair.second.get();
                descr.defined = true;
                auto iter = inherited.find(pair.first);
                if (iter != inherited.end())
                {
                    inherited.erase(iter);
                    descr.redefined = true;
                }
                result.push_back(descr);
            }
            // inherited
            for (auto pair : inherited)
            {
                descriptor descr = {0};
                descr.sl = pair.second.get();
                result.push_back(descr);
            }
            return result;
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
