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

#include "compiler/ast/root.h"
#include "compiler/scanner/scanner.h"
#include "parser_data.h"
#include "parser_data_filler.h"
#include "lib/pointer.h"

namespace g0at
{
    namespace parser
    {
        class parser
        {
        public:
            parser();
            ~parser();
            static lib::pointer<ast::root> parse(scanner *scan, bool debug, const char *prog_name);
            void create_root(scanner *scan);
            void parse();
            lib::pointer<ast::root> get_root() { return root; }

        protected:
            parser(const parser &) { }
            void operator=(const parser &) { }
            static void parse_brackets_and_fill_data(scanner *scan, lib::pointer<ast::token_with_list> dst,
                parser_data_filler *data_filler, wchar_t open_bracket);
            static void parse_function_body(ast::function *func);
            static void parse_function_args(ast::function *func);
            static void parse_function_and_method_call_args(ast::token_list *src, ast::token_list *dst);
            static void parse_function_call_args(ast::function_call *fcall);
            static void parse_method_call_args(ast::method_call *vcall);
            static void parse_object_body(ast::token_object *obj);

            lib::pointer<ast::root> root;
            parser_data *data;
        };
    };
};
