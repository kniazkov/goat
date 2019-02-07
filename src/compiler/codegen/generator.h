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

#include "compiler/pt/node_visitor.h"
#include "code/code.h"
#include "model/name_cache.h"
#include "lib/pointer.h"

namespace g0at
{
    namespace codegen
    {
        class generator : public pt::node_visitor
        {
        public:
            generator();
            static lib::pointer<code::code> generate(lib::pointer<pt::function> node_root);
            void visit(pt::variable *ref) override;
            void visit(pt::function *ref) override;
            void visit(pt::static_string *ref) override;
            void visit(pt::function_call *ref) override;
            void visit(pt::statement_expression *ref) override;
            void visit(pt::addition *ref) override;
            void visit(pt::integer *ref) override;
            void visit(pt::subtraction *ref) override;
            void visit(pt::negation *ref) override;
            void visit(pt::value_void *ref) override;
            void visit(pt::value_undefined *ref) override;
            void visit(pt::value_null *ref) override;
            void visit(pt::declare_variable *ref) override;

        protected:
            lib::pointer<code::code> code; 
            model::name_cache name_cache;
        };
    };
};
