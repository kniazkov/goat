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
            symbol(int _id, std::wstring _name, type *_type);
            symbol(const wchar_t *_name, type *_type);
            virtual ~symbol();

            int get_id() { return id; }
            std::wstring get_name() { return name; }
            type * get_last_type() { return last_type; }
            inline int get_proto_count();
            inline type *get_proto(int idx);

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
            type(lib::pointer<type> _proto);
            virtual ~type();
            int get_proto_count() { return (int)proto.size(); }
            type *get_proto(int idx) { return proto.at(idx); }

        protected:
            std::vector<type*> proto;
            std::map<std::wstring, lib::pointer<symbol>> symbols;
        };

        class type_object : public type
        {
        public:
            type_object();
        };

        class type_function_proto : public type
        {
        public:
            type_function_proto(lib::pointer<type_object> _proto);
        };

        class type_function : public type
        {
        public:
            type_function(lib::pointer<type_function_proto> _proto);

        protected:
            std::vector<lib::pointer<symbol>> args;
            std::vector<type*> allowed_ret_types;
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
            virtual ~scope();
            std::vector<descriptor> get_symbol_table();
            symbol *find_symbol_by_type(type *_type);

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

        /*
            Inline methods
        */
        int symbol::get_proto_count()
        {
            return last_type ? last_type->get_proto_count() : 0;
        }

        type * symbol::get_proto(int idx)
        {
            return last_type ? last_type->get_proto(idx) : nullptr;
        }
    };
};
