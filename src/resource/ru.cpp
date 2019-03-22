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

#include "resource.h"
#include "lib/utils.h"
#include <sstream>

namespace g0at
{
    namespace resource
    {
        class ru : public resource
        {
        public:
            static resource *get_instance()
            {
                static ru instance;
                return &instance;
            }

            std::wstring bad_utf8() override
            {
                return L"входные данные не соответствуют кодировке utf-8";
            }

            std::wstring unknown_character(wchar_t ch) override
            {
                std::wstringstream wss;
                wss << L"неизвестный символ '" << ch << L'\'';
                return wss.str();
            }

            std::wstring no_input_file() override
            {
                return L"не указан файл с исходным кодом";
            }

            std::wstring file_not_found(const char *file_name) override
            {
                std::wstringstream wss;
                wss << L"файл \'" << file_name << L"\' не найден";
                return wss.str();
            }

            std::wstring missing_closing_quote() override
            {
                return L"пропущена закрывающая кавычка";
            }

            std::wstring invalid_escape_sequence(wchar_t ch) override
            {
                std::wstringstream wss;
                wss << L"неправильная управляющая последовательность '\\" << ch << L'\'';
                return wss.str();
            }

            std::wstring incorrect_command_line_parameter(const char *parameter) override
            {
                std::wstringstream wss;
                wss << L"неправильный параметр командной строки: '" << parameter << L'\'';
                return wss.str();
            }

            std::wstring unable_to_parse_token_sequence() override
            {
                return L"невозможно разобрать последовательность токенов";
            }

            std::wstring invald_lvalue_expression() override
            {
                return L"неправильное левостороннее выражение для оператора присваивания";
            }

            std::wstring expected_an_expression() override
            {
                return L"здесь должно быть выражение";
            }

            std::wstring expected_an_expression_after_operator() override
            {
                return L"после оператора должно быть выражение";
            }

            std::wstring expected_an_identifier() override
            {
                return L"ожидается идентификатор";
            }

            std::wstring the_next_token_must_be_a_comma_or_a_semicolon() override
            {
                return L"следующий токен должен быть запятой или точкой с запятой";
            }

            std::wstring function_arguments_must_be_separated_by_commas() override
            {
                return L"аргументы функции должны разделяться запятыми";
            }

            std::wstring expected_an_argument_list() override
            {
                return L"ожидается список аргументов";
            }

            std::wstring expected_a_function_body() override
            {
                return L"ожидается тело функции";
            }

            std::wstring the_next_token_must_be_a_semicolon() override
            {
                return L"следующий токен должен быть точкой с запятой";
            }

            std::wstring the_next_token_must_be_a_colon() override
            {
                return L"следующий токен должен быть двоеточием";
            }

            std::wstring key_must_be_separated_by_a_colon() override
            {
                return L"ключ и значение должны разделяться двоеточием";
            }

            std::wstring pairs_must_be_separated_by_commas() override
            {
                return L"пары ключ:значение должны разделяться запятыми";
            }

            std::wstring expected_an_expression_before_dot() override
            {
                return L"перед оператором '.' должно быть выражение";
            }

            std::wstring expected_an_identifier_after_dot() override
            {
                return L"после оператора '.' должен быть идентификатор";
            }

            std::wstring expected_a_statement() override
            {
                return L"ожидается инструкция";
            }

            std::wstring expected_a_conditional_expression() override
            {
                return L"ожидается выражение-условие";
            }

            std::wstring is_not_binary_file() override
            {
                return L"это не исполняемый файл goat";
            }

            std::wstring file_is_corrupted() override
            {
                return L"файл повреждён";
            }

            std::wstring index_out_of_bounds() override
            {
                return L"индекс вышел за пределы массива";
            }

            std::wstring memory_leak(int blocks_count, size_t size) override
            {
                std::wstringstream wss;
                wss << L"утечка памяти; блоков: " << blocks_count << L", байт: " << size;
                return wss.str();
            }

            virtual std::wstring memory_usage_report(size_t heap_size, size_t max_size,
                const wchar_t *gc_algorithm, int gc_count_launches,
                uint64_t total_objects, uint64_t created_objects,
                uint64_t reused_objects, double reused_percent) override
            {
                std::wstringstream wss;
                wss << L"Отчет о расходовании памяти:  " << std::endl <<
                    L"   размер кучи, байт:            " << heap_size << std::endl <<
                    L"   пиковое значение, байт:       " << max_size << std::endl <<
                    L"   алгоритм сборщика мусора:     '" << gc_algorithm << L'\'' << std::endl <<
                    L"   число итераций сборки мусора: " << gc_count_launches << std::endl <<
                    L"   всего объектов:               " << total_objects << std::endl <<
                    L"      реально созданных:         " << created_objects << std::endl <<
                    L"      повторно использованных:   " << reused_objects << L" (" << lib::double_to_wstring(reused_percent, 1) << L" %)";
                return wss.str();
            }

            std::wstring assertion_failed(const char *file, int line, const char *assertion) override
            {
                std::wstringstream wss;
                wss << file << L", " << line << L", утверждение ложно: \'" << assertion << L'\'';
                return wss.str();
            }

            std::wstring objects_must_be_separated_by_commas() override
            {
                return L"объекты должны разделяться запятыми";
            }

            std::wstring unclosed_comment() override
            {
                return L"не закрыт многострочный комментарий";
            }



            std::wstring unhandled_exception(std::wstring str) override
            {
                std::wstringstream wss;
                wss << L"необработанное исключение: " << str;
                return wss.str();
            }

            std::wstring unknown_exception() override
            {
                return L"неизвестный тип исключения";
            }

            std::wstring illegal_argument() override
            {
                return L"недопустимый аргумент";
            }        
        };

        resource *resource::get_intance_ru()
        {
            return ru::get_instance();
        }
    };
};