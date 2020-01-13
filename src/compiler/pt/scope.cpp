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

        symbol::symbol(int _id, std::wstring _name, type *_type)
            : id(_id), name(_name), last_type(_type)
        {
        }

        symbol::symbol(const wchar_t *_name, type *_type)
            : id(-1), name(_name), last_type(_type)
        {
        }

        symbol::~symbol()
        {
        }


        type::type()
        {
        }

        type::type(lib::pointer<type> _proto)
        {
            proto.push_back(_proto.get());
        }

        type::~type()
        {
        }

        type_object::type_object()
        {
        }

        type_function_proto::type_function_proto(lib::pointer<type_object> _proto)
            : type(_proto.cast<type>())
        {
        }

        type_function::type_function(lib::pointer<type_function_proto> _proto)
            : type(_proto.cast<type>())
        {
        }


        scope::scope()
        {
        }

        scope::scope(lib::pointer<scope> &_parent)
        {
            parents.push_back(_parent);
        }

        scope::~scope()
        {
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

        symbol * scope::find_symbol_by_type(type *_type)
        {
            for (auto pair : symbols)
            {
                if (pair.second->get_last_type() == _type)
                    return pair.second.get();
            }
            
            symbol *sl = nullptr;
            for (auto pt : parents)
            {
                sl = pt->find_symbol_by_type(_type);
                if (sl != nullptr)
                    break;
            }
            return sl;
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
