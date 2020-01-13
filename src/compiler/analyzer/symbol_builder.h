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

#include "analyzer.h"
#include "compiler/pt/parse_tree_traversal.h"
#include "compiler/pt/root_scope/root_scope.h"

namespace g0at
{
    namespace analyzer
    {
        class symbol_builder : public pt::parse_tree_traversal
        {
            public:
                symbol_builder(pt::root_scope::root_scope *_root_scope);

            protected:
                void payload(pt::declare_variable *ref) override;
                void payload(pt::integer *ref) override;

                int uid;
                pt::root_scope::root_scope *root_scope;
        };
    };
};