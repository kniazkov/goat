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

#include "../pt/function.h"
#include "../ast/root.h"
#include "../../lib/pointer.h"

namespace g0at
{
    namespace analyzer
    {
        class analyzer
        {
        public:
            analyzer();
            ~analyzer();
            static lib::pointer<pt::function> analyze(lib::pointer<ast::root> tok_root);
            void build(lib::pointer<ast::root> tok_root);
            lib::pointer<pt::function> get_root() { return root; }

        protected:
            analyzer(const analyzer &) { }
            void operator=(const analyzer &) { }
            lib::pointer<pt::function> build_function(lib::pointer<ast::function> tok_func);

            lib::pointer<pt::function> root;
        };
    };
};
