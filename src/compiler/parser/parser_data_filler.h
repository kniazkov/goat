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

#include "compiler/ast/token_visitor.h"
#include "parser_data.h"

namespace g0at
{
    namespace parser
    {
        class parser_data_filler : public ast::token_visitor
        {
        public:
            parser_data_filler(parser_data *_data);
            void visit(ast::identifier *ref) override;
            void visit(ast::plus *ref) override;
            void visit(ast::minus *ref) override;
            void visit(ast::static_string *ref) override;
            void visit(ast::integer *ref) override;
            void visit(ast::real *ref) override;
            void visit(ast::keyword_var *ref) override;
            void visit(ast::assign *ref) override;
            void visit(ast::keyword_function *ref) override;
            void visit(ast::keyword_thread *ref) override;
            void visit(ast::keyword_return *ref) override;
            void visit(ast::brackets_pair *ref) override;
            void visit(ast::dot *ref) override;
            void visit(ast::equals *ref) override;
            void visit(ast::not_equal *ref) override;
            void visit(ast::keyword_while *ref) override;
            void visit(ast::value_void *ref) override;
            void visit(ast::value_undefined *ref) override;
            void visit(ast::value_null *ref) override;
            void visit(ast::value_true *ref) override;
            void visit(ast::value_false *ref) override;
            void visit(ast::this_ptr *ref) override;
            void visit(ast::keyword_if *ref) override;
            void visit(ast::keyword_throw *ref) override;
            void visit(ast::keyword_try *ref) override;
            void visit(ast::inherit *ref) override;
            void visit(ast::character *ref) override;
            void visit(ast::keyword_for *ref) override;
            void visit(ast::less *ref) override;
            void visit(ast::semicolon *ref) override;
            void visit(ast::keyword_new *ref) override;
            void visit(ast::increment *ref) override;
            void visit(ast::keyword_lock *ref) override;
            void visit(ast::keyword_in *ref) override;
            void visit(ast::keyword_do *ref) override;
            void visit(ast::keyword_break *ref) override;
            void visit(ast::keyword_continue *ref) override;
            void visit(ast::keyword_switch *ref) override;
            void visit(ast::decrement *ref) override;
            void visit(ast::asterisk *ref) override;
            void visit(ast::double_asterisk *ref) override;
            void visit(ast::slash *ref) override;
            void visit(ast::percent *ref) override;
            void visit(ast::exclamation *ref) override;
            void visit(ast::tilde *ref) override;
            void visit(ast::double_exclamation *ref) override;
            void visit(ast::double_less *ref) override;
            void visit(ast::double_greater *ref) override;
            void visit(ast::triple_greater *ref) override;
            void visit(ast::less_or_equal *ref) override;
            void visit(ast::greater *ref) override;
            void visit(ast::greater_or_equal *ref) override;
            void visit(ast::ampersand *ref) override;
            void visit(ast::double_ampersand *ref) override;
            void visit(ast::vertical_bar *ref) override;
            void visit(ast::double_vertical_bar *ref) override;
            void visit(ast::caret *ref) override;
            void visit(ast::plus_assign *ref) override;
            void visit(ast::minus_assign *ref) override;
            void visit(ast::asterisk_assign *ref) override;
            void visit(ast::slash_assign *ref) override;
            void visit(ast::percent_assign *ref) override;
            void visit(ast::double_less_assign *ref) override;
            void visit(ast::double_greater_assign *ref) override;
            void visit(ast::triple_greater_assign *ref) override;
            void visit(ast::ampersand_assign *ref) override;
            void visit(ast::vertical_bar_assign *ref) override;
            void visit(ast::caret_assign *ref) override;
            void visit(ast::question_mark *ref) override;
            void visit(ast::keyword_debug *ref) override;
            void visit(ast::protect *ref) override;
            void visit(ast::question_with_dot *ref) override;

        protected:
            parser_data *data;
        };
    };
};
