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

#include "compiler/pt/node_visitor.h"
#include "code/code.h"
#include "model/name_cache.h"
#include "lib/pointer.h"
#include "lib/ref_counter.h"

namespace g0at
{
    namespace codegen
    {
        class generator;

        class lvalue_generator : public pt::node_visitor, public lib::ref_counter
        {
        public:
            lvalue_generator(lib::pointer<code::code> _code, model::name_cache *_name_cache, generator *_rgen);
            void visit(pt::variable *ref) override;
            void visit(pt::property *ref) override;
            void visit(pt::index_access *ref) override;

        protected:
            lib::pointer<code::code> code; 
            model::name_cache *name_cache;
            generator *rgen;
        };
    };
};
