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

#include "../ast/root.h"
#include "../scanner/scanner.h"
#include <memory>

namespace g0at
{
    class parser
    {
    public:
        parser();
        void create_root(scanner *scan);
        std::shared_ptr<ast::root> get_root() { return root; }

    protected:
        parser(const parser &) { }
        void operator=(const parser &) { }

        std::shared_ptr<ast::root> root;
    };
};
