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

#include <string>

namespace g0at
{
    namespace resource
    {
        class resource
        {
        public:
            virtual ~resource() { }
            static resource *get_instance(const char *lang);

            virtual std::wstring bad_utf8() = 0;
            virtual std::wstring unknown_character(wchar_t ch) = 0;
            virtual std::wstring no_input_file() = 0;
            virtual std::wstring file_not_found(const char *file_name) = 0;
            virtual std::wstring missing_closing_quote() = 0;
            virtual std::wstring invalid_escape_sequence(wchar_t ch) = 0;
            virtual std::wstring incorrect_command_line_parameter(const char *parameter) = 0;
            virtual std::wstring unable_to_parse_token_sequence() = 0;
            virtual std::wstring invald_lvalue_expression() = 0;
            virtual std::wstring expected_an_expression() = 0;
            virtual std::wstring expected_an_expression_after_operator() = 0;
            virtual std::wstring expected_an_identifier() = 0;
            virtual std::wstring the_next_token_must_be_a_comma_or_a_semicolon() = 0;
            virtual std::wstring function_arguments_must_be_separated_by_commas() = 0;
            virtual std::wstring expected_an_argument_list() = 0;
            virtual std::wstring expected_a_function_body() = 0;
            virtual std::wstring the_next_token_must_be_a_semicolon() = 0;
            virtual std::wstring the_next_token_must_be_a_colon() = 0;
            virtual std::wstring key_must_be_separated_by_a_colon() = 0;
            virtual std::wstring pairs_must_be_separated_by_commas() = 0;
            virtual std::wstring expected_an_expression_before_dot() = 0;
            virtual std::wstring expected_an_identifier_after_dot() = 0;
            virtual std::wstring expected_a_statement() = 0;
            virtual std::wstring expected_a_conditional_expression() = 0;
            virtual std::wstring is_not_binary_file() = 0;
            virtual std::wstring file_is_corrupted() = 0;
            virtual std::wstring index_out_of_bounds() = 0;
            virtual std::wstring memory_leak(int blocks_count, size_t size) = 0;
            virtual std::wstring memory_usage_report(size_t heap_size, size_t max_size,
                const wchar_t *gc_algorithm, int gc_count_launches,
                uint64_t total_objects, uint64_t created_objects,
                uint64_t reused_objects, double reused_percent) = 0;
            virtual std::wstring assertion_failed(const char *file, int line,
                const char *assertion) = 0;
            virtual std::wstring objects_must_be_separated_by_commas() = 0;
            virtual std::wstring unclosed_comment() = 0;
            virtual std::wstring the_next_block_must_be_a_catch_or_finally() = 0;
            virtual std::wstring character_constant_too_long_for_its_type() = 0;
            virtual std::wstring expected_parameters_of_cycle_statement() = 0;
            virtual std::wstring expected_a_var_keyword() = 0;
            virtual std::wstring expected_a_while_keyword() = 0;
            virtual std::wstring expected_a_switch_body() = 0;
            virtual std::wstring expected_case_or_default_keyword() = 0;
            virtual std::wstring can_have_only_one_default_block() = 0;
            virtual std::wstring expected_a_file_name() = 0;
            virtual std::wstring wrong_file_name(std::wstring file_name) = 0;

            virtual std::wstring unhandled_exception(std::wstring str) = 0;
            virtual std::wstring unknown_exception() = 0;
            virtual std::wstring illegal_argument() = 0;
            virtual std::wstring illegal_context() = 0;
            virtual std::wstring illegal_operation() = 0;
            virtual std::wstring illegal_reference() = 0;
            virtual std::wstring illegal_type() = 0;
            virtual std::wstring operator_not_found(std::wstring oper) = 0;
            virtual std::wstring is_not_a_function(std::wstring name) = 0;
            virtual std::wstring is_not_a_method(std::wstring name) = 0;
            
        protected:
            static resource *get_intance_en();
            static resource *get_intance_ru();
        };
    };
};