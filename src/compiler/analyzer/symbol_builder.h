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

#include "analyzer.h"
#include "compiler/pt/parse_tree_traversal.h"

namespace g0at
{
    namespace analyzer
    {
        class built_in_types
        {
        friend class analyzer;
        public:
            built_in_types();
            pt::type *get_type_integer() { return type_integer; }

        private:
            pt::type *type_integer;
        };

        class symbol_builder : public pt::parse_tree_traversal, public built_in_types
        {
            public:
                symbol_builder();

            protected:
                void payload(pt::declare_variable *ref) override;
                void payload(pt::integer *ref) override;

                int uid;
        };
    };
};