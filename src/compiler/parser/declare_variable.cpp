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
#include "common_exceptions.h"
#include "compiler/ast/keyword_var.h"
#include "compiler/ast/variable.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/comma.h"
#include "compiler/ast/semicolon.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/assignment.h"
#include "compiler/ast/declare_variable.h"
#include "lib/assert.h"

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
            bool check(ast::token *tok) override
            {
                ast::keyword_var *kw = tok->to_keyword_var();
                assert(kw != nullptr);
                
                lib::pointer<ast::declare_variable> stmt = new ast::declare_variable(kw);

                ast::token *begin_decl = tok;
                ast::semicolon *semicolon = nullptr;

                while(!semicolon)
                {
                    if(!begin_decl->next)
                        throw expected_an_identifier(begin_decl->get_fragment().end);
                    
                    ast::identifier *name = begin_decl->next->to_identifier();
                    if (name != nullptr)
                    {
                        if (!name->next)
                            throw the_next_token_must_be_a_comma_or_a_semicolon(name->get_fragment().end);

                        ast::comma *comma = name->next->to_comma();
                        if (!comma)
                        {
                            semicolon = name->next->to_semicolon();
                            if (!semicolon)
                                throw the_next_token_must_be_a_comma_or_a_semicolon(name->next->get_fragment().end);
                        }
                        else
                        {
                            begin_decl = comma;
                        }
                        
                        ast::variable_info var_info;
                        var_info.name = name->get_name();
                        stmt->add_variable(var_info);
                    }
                    else
                    {
                        ast::assignment *at = begin_decl->next->to_assignment();
                        if (!at)
                            throw expected_an_identifier(begin_decl->next->get_fragment().begin);
                        
                        ast::variable *name_as_var = at->get_left()->to_variable();
                        if (!name_as_var)
                            throw expected_an_identifier(begin_decl->next->get_fragment().begin);
                        name = name_as_var->to_identifier();
                        assert(name != nullptr);

                        if (!at->next)
                            throw the_next_token_must_be_a_comma_or_a_semicolon(at->get_right()->get_fragment().end);

                        ast::comma *comma = at->next->to_comma();
                        if (!comma)
                        {
                            semicolon = at->next->to_semicolon();
                            if (!semicolon)
                                throw the_next_token_must_be_a_comma_or_a_semicolon(at->get_right()->get_fragment().end);
                        }
                        else
                        {
                            begin_decl = comma;
                        }

                        ast::variable_info var_info;
                        var_info.name = name->get_name();
                        var_info.init_val = at->get_right();
                        stmt->add_variable(var_info);
                    }
                }

                kw->replace(semicolon, stmt.cast<ast::token>());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_declare_variable()
        {
            return new declare_variable(data);
        }
    };
};
