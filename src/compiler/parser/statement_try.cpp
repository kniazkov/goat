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

#include "pattern_right_to_left.h"
#include "grammar_factory.h"
#include "common_exceptions.h"
#include "compiler/ast/keyword_try.h"
#include "compiler/ast/keyword_catch.h"
#include "compiler/ast/keyword_finally.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/statement.h"
#include "compiler/ast/statement_try.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class the_next_block_must_be_a_catch_or_finally : public compilation_error
        {
        public:
            the_next_block_must_be_a_catch_or_finally(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->the_next_block_must_be_a_catch_or_finally())
            {
            }
        };

        class statement_try : public pattern_right_to_left
        {
        public:
            statement_try(parser_data *_data)
                : pattern_right_to_left(&_data->try_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_try *kw = tok->to_keyword_try();
                assert(kw != nullptr);

                if (!kw->next)
                    throw expected_a_statement(kw->get_position());

                ast::statement *stmt_try = kw->next->to_statement();
                if (!stmt_try)
                    throw expected_a_statement(kw->next->get_position());

                if (!stmt_try->next)
                    throw the_next_block_must_be_a_catch_or_finally(stmt_try->get_position());

                ast::keyword *kw_catch = stmt_try->next->to_keyword_catch();
                if (kw_catch)
                {
                    lib::pointer<ast::token> kw_catch_next = kw_catch->next;

                    if (!kw_catch_next)
                        throw expected_a_statement(kw_catch->get_position());

                    std::wstring var_name = L"";
                    ast::brackets_pair *brackets = kw_catch->next->to_brackets_pair();
                    if (brackets && brackets->get_symbol() == L'(')
                    {
                        ast::token_list *list = brackets->get_raw_list();
                        if (list->is_empty())
                            throw expected_an_identifier(brackets->get_position());
                        if (!list->has_only_one_item())
                            throw unable_to_parse_token_sequence(brackets->get_position());
                        ast::identifier *ident = list->first->to_identifier();
                        if (!ident)
                            throw expected_an_identifier(list->first->get_position());
                        var_name = ident->get_name();
                        kw_catch_next = kw_catch_next->next;
                        if (!kw_catch_next)
                            throw expected_a_statement(brackets->get_position());
                    }
                    
                    ast::statement *stmt_catch = kw_catch_next->to_statement();
                    if (!stmt_catch)
                        throw expected_a_statement(kw_catch_next->get_position());
                    
                    do
                    {
                        if (!stmt_catch->next)
                            break;
                        
                        ast::keyword_finally *kw_finally = stmt_catch->next->to_keyword_finally();
                        if (!kw_finally)
                            break;

                        if (!kw_finally->next)
                            throw expected_a_statement(kw_finally->get_position());

                        ast::statement *stmt_finally = kw_finally->next->to_statement();
                        if (!stmt_finally)
                            throw expected_a_statement(kw_finally->next->get_position());

                        lib::pointer<ast::token> result = new ast::statement_try(kw, stmt_try, stmt_catch, var_name, stmt_finally);
                        kw->replace(stmt_finally, result);
                        return false;
                    } while(false);

                    lib::pointer<ast::token> result = new ast::statement_try(kw, stmt_try, stmt_catch, var_name);
                    kw->replace(stmt_catch, result);
                    return false;
                }

                ast::keyword *kw_finally = stmt_try->next->to_keyword_finally();
                if (!kw_finally)
                    throw the_next_block_must_be_a_catch_or_finally(stmt_try->get_position());

                if (!kw_finally->next)
                    throw expected_a_statement(kw_finally->get_position());

                ast::statement *stmt_finally = kw_finally->next->to_statement();
                if (!stmt_finally)
                    throw expected_a_statement(kw_finally->next->get_position());

                lib::pointer<ast::token> result = new ast::statement_try(kw, stmt_try, stmt_finally);
                kw->replace(stmt_finally, result);

                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_try()
        {
            return new statement_try(data);
        }
    };
};
