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

#include "lvalue_generator.h"
#include "generator.h"
#include "compiler/pt/variable.h"
#include "compiler/pt/property.h"
#include "compiler/pt/index_access.h"
#include "code/store.h"
#include "code/write.h"
#include "code/set.h"

namespace g0at
{
    namespace codegen
    {
        lvalue_generator::lvalue_generator(lib::pointer<code::code> _code, model::name_cache *_name_cache, generator *_rgen)
            : code(_code), name_cache(_name_cache), rgen(_rgen)
        {
        }

        void lvalue_generator::visit(pt::variable *ref)
        {
            int id = name_cache->get_id(ref->get_name());
            code->add_instruction(new code::_store(id));
        }

        void lvalue_generator::visit(pt::property *ref)
        {
            ref->get_left()->accept(rgen);
            int id = name_cache->get_id(ref->get_name());
            code->add_instruction(new code::_write(id));
        }

        void lvalue_generator::visit(pt::index_access *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(rgen);
            }
            ref->get_object()->accept(rgen);
            code->add_instruction(new code::_set(args_count + 1));
        }
    };
};