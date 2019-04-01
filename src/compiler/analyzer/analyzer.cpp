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

#include "analyzer.h"
#include "statement_builder.h"
#include "lib/assert.h"

namespace g0at
{
    namespace analyzer
    {
        analyzer::analyzer()
            : root(nullptr)
        {
        }

        analyzer::~analyzer()
        {
        }

        lib::pointer<pt::function> analyzer::analyze(lib::pointer<ast::root> tok_root)
        {
            analyzer aobj;
            aobj.build(tok_root);
            return aobj.get_root();
        }

        void analyzer::build(lib::pointer<ast::root> tok_root)
        {
            root = build_function(tok_root.cast<ast::function>());
        }

        lib::pointer<pt::function> analyzer::build_function(lib::pointer<ast::function> tok_func)
        {
            lib::pointer<pt::function> node_func = new pt::function(tok_func->get_position());
            
            auto body = tok_func->get_body();
            auto tok = body->first;
            while(tok)
            {
                statement_builder visitor;
                tok->accept(&visitor);
                assert(visitor.has_stmt());
                node_func->add_stmt(visitor.get_stmt());
                tok = tok->next;
            }

            return node_func;
        }
    };
};