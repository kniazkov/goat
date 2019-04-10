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
#include "scope_builder.h"
#include "symbol_builder.h"
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

        lib::pointer<pt::function> analyzer::analyze(lib::pointer<ast::root> root_tok)
        {
            analyzer aobj;
            aobj.build(root_tok);
            return aobj.get_root();
        }

        void analyzer::build(lib::pointer<ast::root> root_tok)
        {
            // build parse tree from the AST
            pt::function *root_node = new pt::function(root_tok->get_position());
            root = root_node;
            
            auto body = root_tok->get_body();
            auto tok = body->first;
            while(tok)
            {
                statement_builder b0;
                tok->accept(&b0);
                assert(b0.has_stmt());
                root_node->add_stmt(b0.get_stmt());
                tok = tok->next;
            }

            // root scope
            lib::pointer<pt::scope> rsk = new pt::scope();
            built_in_types bt;

            // create scope for each node
            scope_builder b1(rsk);
            root_node->accept(&b1);

            // create symbols
            symbol_builder b2;
            fill_root_scope(rsk.get(), &b2);
            b2.traverse(root_node);
        }

        void analyzer::fill_root_scope(pt::scope *sk, built_in_types *bt)
        {
            /*
                Integer
            */
            lib::pointer<pt::type> type_integer = new pt::type();
            bt->type_integer = type_integer.get();
            sk->add_type(type_integer);
            lib::pointer<pt::symbol> sl_integer = new pt::symbol(L"Integer", type_integer.get());
            sk->add_symbol(sl_integer);
        }
    };
};