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

#include "node.h"
#include "compiler/analyzer/symbol_builder.h"

namespace g0at
{
    namespace pt
    {
        class expression : public node
        {
        friend class g0at::analyzer::scope_builder;
        public:
            expression(lib::pointer<position> _pos);
            expression *to_expression() override;

            lib::pointer<type> get_type() { return ret_type; }

        protected:
            void set_type(lib::pointer<type> _type) { ret_type = _type; }
        
        private:
            lib::pointer<type> ret_type;
        };
    };
};