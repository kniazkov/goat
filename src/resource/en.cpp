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

#include "resource.h"
#include "lib/utils.h"
#include <sstream>

namespace g0at
{
    namespace resource
    {
        class en : public resource
        {
        public:
            static resource *get_instance()
            {
                static en instance;
                return &instance;
            }

            std::wstring bad_utf8() override
            {
                return L"input data is not utf-8 encoded";
            }

            std::wstring unknown_character(wchar_t ch) override
            {
                std::wstringstream wss;
                wss << L"unknown character '" << ch << L'\'';
                return wss.str();
            }

            std::wstring no_input_file() override
            {
                return L"no input file";
            }

            std::wstring expected_a_hex_digit() override
            {
                return L"expected_a_hex_digit";
            }

            std::wstring expected_0_or_1() override
            {
                return L"expected '0' or '1'";
            }

            std::wstring identifier_starts_after_numeric_literal() override
            {
                return L"identifier starts immediately after numeric literal";
            }

            std::wstring file_not_found(std::string file_name) override
            {
                std::wstringstream wss;
                wss << L"file \'" << file_name.c_str() << L"\' not found";
                return wss.str();
            }

            std::wstring missing_closing_quote() override
            {
                return L"missing a closing quote";
            }

            std::wstring invalid_escape_sequence(wchar_t ch) override
            {
                std::wstringstream wss;
                wss << L"invalid escape sequence '\\" << ch << L'\'';
                return wss.str();
            }

            std::wstring incorrect_command_line_parameter(const char *parameter) override
            {
                std::wstringstream wss;
                wss << L"incorrect command line parameter: '" << parameter << L'\'';
                return wss.str();
            }

            std::wstring unable_to_parse_token_sequence() override
            {
                return L"unable to parse token sequence";
            }

            std::wstring invald_lvalue_expression() override
            {
                return L"invalid lvalue expression for the assingment operator";
            }

            std::wstring expected_an_expression() override
            {
                return L"expected an expression";
            }

            std::wstring expected_an_expression_after_operator() override
            {
                return L"expected an expression after operator";
            }

            std::wstring expected_an_identifier() override
            {
                return L"expected an identifier";
            }

            std::wstring the_next_token_must_be_a_comma_or_a_semicolon() override
            {
                return L"the next token must be a comma or a semicolon";
            }

            std::wstring function_arguments_must_be_separated_by_commas() override
            {
                return L"function arguments must be separated by commas";
            }

            std::wstring expected_an_argument_list() override
            {
                return L"expected an argument list";
            }

            std::wstring expected_a_function_body() override
            {
                return L"expected a function body";
            }

            std::wstring the_next_token_must_be_a_semicolon() override
            {
                return L"the next token must be a semicolon";
            }

            std::wstring the_next_token_must_be_a_colon() override
            {
                return L"the next token must be a colon";
            }

            std::wstring key_must_be_separated_by_a_colon() override
            {
                return L"the key must be separated from the value by a colon";
            }

            std::wstring pairs_must_be_separated_by_commas() override
            {
                return L"pairs of keys and values must be separated by commas";
            }

            std::wstring expected_an_expression_before_dot() override
            {
                return L"expected a primary expression before '.' operator";
            }

            std::wstring expected_an_identifier_after_dot() override
            {
                return L"expected an identifier after '.' operator";
            }

            std::wstring expected_a_statement() override
            {
                return L"expected a statement";
            }

            std::wstring expected_a_conditional_expression() override
            {
                return L"expected a conditional expression";
            }

            std::wstring is_not_binary_file() override
            {
                return L"is not goat binary file";
            }

            std::wstring file_is_corrupted() override
            {
                return L"file is corrupted";
            }

            std::wstring index_out_of_bounds() override
            {
                return L"index out of bounds";
            }

            std::wstring memory_leak(int blocks_count, size_t size) override
            {
                std::wstringstream wss;
                wss << L"memory leak; blocks: " << blocks_count << L", bytes: " << size;
                return wss.str();
            }

            virtual std::wstring memory_usage_report(size_t heap_size, size_t max_size,
                const wchar_t *gc_algorithm, int gc_count_launches,
                uint64_t total_objects, uint64_t created_objects,
                uint64_t reused_objects, double reused_percent) override
            {
                std::wstringstream wss;
                wss << L"MEMORY USAGE REPORT" << std::endl << std::endl <<
                    L"heap size, bytes:       " << heap_size << std::endl <<
                    L"peak usage, bytes:      " << max_size << std::endl <<
                    L"gc algorithm:           '" << gc_algorithm << L'\'' << std::endl <<
                    L"count of gc launches:   " << gc_count_launches << std::endl <<
                    L"total objects:          " << total_objects << std::endl <<
                    L"- actually created:     " << created_objects << std::endl <<
                    L"- reused:               " << reused_objects << L" (" << lib::double_to_wstring(reused_percent, 1) << L" %)";
                return wss.str();
            }

            std::wstring assertion_failed(const char *file, int line, const char *assertion) override
            {
                std::wstringstream wss;
                wss << file << L", " << line << L", assertion failed: \'" << assertion << L'\'';
                return wss.str();
            }

            std::wstring objects_must_be_separated_by_commas() override
            {
                return L"objects must be separated by commas";
            }

            std::wstring unclosed_comment() override
            {
                return L"unclosed multiline comment";
            }

            std::wstring the_next_block_must_be_a_catch_or_finally() override
            {
                return L"the next construction must be a 'catch' or 'finally' block";
            }

            std::wstring character_constant_too_long_for_its_type() override
            {
                return L"character constant too long for its type";
            }

            std::wstring expected_parameters_of_cycle_statement() override
            {
                return L"expected parameters of 'for' statement";
            }

            std::wstring expected_a_var_keyword() override
            {
                return L"here can be only a 'var' keyword";
            }

            std::wstring expected_a_while_keyword() override
            {
                return L"expected a 'while' keyword";
            }

            std::wstring expected_a_switch_body() override
            {
                return L"expected a body of the 'switch' statement";
            }

            std::wstring expected_case_or_default_keyword() override
            {
                return L"expected one of: 'case' or 'default' keyword";
            }

            std::wstring can_have_only_one_default_block() override
            {
                return L"a 'switch' statement can have only one 'default' block";
            }

            std::wstring expected_a_file_name() override
            {
                return L"expected a file name";
            }

            std::wstring wrong_file_name(std::wstring file_name) override
            {
                std::wstringstream wss;
                wss << L"wrond file name: '"<< file_name << L'\'';
                return wss.str();
            }



            std::wstring unhandled_exception(std::wstring str) override
            {
                std::wstringstream wss;
                wss << L"unhandled exception: "<< str;
                return wss.str();
            }

            std::wstring unknown_exception() override
            {
                return L"unknown exception";
            }

            std::wstring illegal_argument() override
            {
                return L"illegal argument";
            }

            std::wstring illegal_context() override
            {
                return L"illegal context";
            }

            std::wstring illegal_operation() override
            {
                return L"illegal operation";
            }

            std::wstring division_by_zero() override
            {
                return L"division by zero";
            }

            std::wstring illegal_reference() override
            {
                return L"illegal reference";
            }

            std::wstring illegal_type() override
            {
                return L"illegal type";
            }

            std::wstring operator_not_found(std::wstring oper) override
            {
                std::wstringstream wss;
                wss << L"operator '" << oper << L"' not found";
                return wss.str();
            }

            std::wstring is_not_a_function(std::wstring name) override
            {
                std::wstringstream wss;
                wss << L'\'' << name << L"' is not a function";
                return wss.str();
            }

            std::wstring is_not_a_method(std::wstring name) override
            {
                std::wstringstream wss;
                wss << L'\'' << name << L"' is not a method";
                return wss.str();
            }

            std::wstring undeclared_variable(std::wstring name) override
            {
                std::wstringstream wss;
                wss << L"undeclared variable: '" << name << L'\'';
                return wss.str();
            }



            std::wstring vm_fault() override
            {
                return L"VM fault";
            }

            std::wstring stack_is_empty() override
            {
                return L"stack is empty";
            }

#ifdef STACK_DEBUG
            std::wstring uninitialized_value_on_the_stack() override
            {
                return L"uninitialized value on the stack";
            }
#endif

#ifdef GC_DEBUG
            std::wstring use_of_a_dead_object(std::wstring data) override
            {
                return std::wstring(L"use of a dead object: ") + data;
            }
#endif

            std::wstring setting_breakpoint_at(std::wstring position) override
            {
                std::wstringstream wss;
                wss << L"setting breakpoint at " << position;
                return wss.str();
            }

            std::wstring can_not_set_breakpoint() override
            {
                return L"can not set breakpoint: source not found";
            }

            std::wstring syntax_error() override
            {
                return L"syntax error";
            }

            std::wstring breakpoint_is_hit(std::wstring position) override
            {
                std::wstringstream wss;
                wss << position << L", breakpoint is hit";
                return wss.str();
            }

            std::wstring program_terminated_with_exit_code(int code) override
            {
                std::wstringstream wss;
                wss << L"program terminated with exit code " << code;
                return wss.str();
            }

            std::wstring illegal_reference_variable_is_not_defined(std::wstring name) override
            {
                std::wstringstream wss;
                wss << L"illegal reference: variable '" << name << "' is not defined";
                return wss.str();
            }
        };

        resource *resource::get_intance_en()
        {
            return en::get_instance();
        }
    };
};
