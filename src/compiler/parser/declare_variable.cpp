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

#include "pattern.h"
#include "grammar_factory.h"
#include "compiler/ast/keyword_var.h"
#include "compiler/ast/variable.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/semicolon.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/assignment.h"
#include "compiler/ast/declare_variable.h"
#include <assert.h>

namespace g0at
{
    namespace parser
    {
        class declare_variable : public pattern
        {
        public:
            declare_variable(parser_data *_data)
                : pattern(&_data->var_keywords, _data)
            {
            }

        protected:
            int check(ast::token *tok) override
            {
                ast::keyword_var *kw = tok->to_keyword_var();
                assert(kw != nullptr);
                
                lib::pointer<ast::declare_variable> stmt = new ast::declare_variable(kw);

                assert(kw->next != nullptr); // exception, expected identifier
                ast::identifier *name = kw->next->to_identifier();
                if (name != nullptr)
                {
                    assert(name->next != nullptr); // exception, expected ; or ,
                    ast::semicolon *semicolon = name->next->to_semicolon();
                    assert(semicolon != nullptr); // exception, expected ;
                    
                    ast::variable_info var_info;
                    var_info.name = name->get_name();
                    stmt->add_variable(var_info);

                    kw->replace(semicolon, stmt.cast<ast::token>());
                }
                else
                {
                    ast::assignment *at = kw->next->to_assignment();
                    assert(at != nullptr);
                    ast::variable *var = at->get_left()->to_variable();
                    assert(var != nullptr);
                    ast::identifier *name = var->to_identifier();
                    lib::pointer<ast::expression> init_val = at->get_right();
                    ast::semicolon *semicolon = at->next->to_semicolon();
                    assert(semicolon != nullptr); // exception, expected ;

                    ast::variable_info var_info;
                    var_info.name = name->get_name();
                    var_info.init_val = init_val;
                    stmt->add_variable(var_info);

                    kw->replace(semicolon, stmt.cast<ast::token>());
                }

                return 0;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_declare_variable()
        {
            return new declare_variable(data);
        }
    };
};
