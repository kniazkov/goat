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

#include "lib/ref_counter.h"
#include "lib/pointer.h"
#include <string>
#include <vector>
#include <map>

namespace g0at
{
    namespace pt
    {
        class type;

        class symbol : public lib::ref_counter
        {
        public:
            symbol(int _id, std::wstring _name);
            symbol(const wchar_t *_name, type *_type);

            int get_id() { return id; }
            std::wstring get_name() { return name; }

        protected:
            int id;
            std::wstring name;
            type *last_type;
            std::vector<type*> allowed_types;
        };

        class type : public lib::ref_counter
        {
        public:
            type();

        protected:
            std::vector<type*> proto;
            std::map<std::wstring, lib::pointer<symbol>> symbols;
        };

        class scope : public lib::ref_counter
        {
        public:
            typedef std::map<std::wstring, lib::pointer<symbol>> dictionary;

            struct descriptor
            {
                symbol *sl;
                bool defined;
                bool redefined;
            };

            scope();
            scope(lib::pointer<scope> &_parent);
            std::vector<descriptor> get_symbol_table();

            void add_symbol(lib::pointer<symbol> _sl) { symbols[_sl->get_name()] = _sl; }
            void add_type(lib::pointer<type> _type) { types.push_back(_type); }
            int get_parents_count() { return (int)parents.size(); }
            scope *get_parent(int idx) { return parents.at(idx).get(); }

        protected:
            void flat(dictionary &dst);

            std::vector<lib::pointer<scope>> parents;
            std::vector<lib::pointer<type>> types;
            dictionary symbols;
        };
    };
};
