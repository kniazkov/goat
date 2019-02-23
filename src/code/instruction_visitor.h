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
    namespace code
    {
        class load_string;
        class load_var;
        class call;
        class pop;
        class end;
        class add;
        class load_integer;
        class sub;
        class neg;
        class load_void;
        class load_undefined;
        class load_null;
        class decl_var;
        class store;
        class load_real;
        class load_func;
        class ret;
        class ret_val;
        class create;

        class instruction_visitor
        {
        public:
            instruction_visitor();
            ~instruction_visitor();
            virtual void visit(load_string *ref) = 0;
            virtual void visit(load_var *ref) = 0;
            virtual void visit(call *ref) = 0;
            virtual void visit(pop *ref) = 0;
            virtual void visit(end *ref) = 0;
            virtual void visit(add *ref) = 0;
            virtual void visit(load_integer *ref) = 0;
            virtual void visit(sub *ref) = 0;
            virtual void visit(neg *ref) = 0;
            virtual void visit(load_void *ref) = 0;
            virtual void visit(load_undefined *ref) = 0;
            virtual void visit(load_null *ref) = 0;
            virtual void visit(decl_var *ref) = 0;
            virtual void visit(store *ref) = 0;
            virtual void visit(load_real *ref) = 0;
            virtual void visit(load_func *ref) = 0;
            virtual void visit(ret *ref) = 0;
            virtual void visit(ret_val *ref) = 0;
            virtual void visit(create *ref) = 0;
        };
    };
};
