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
        };

        resource *resource::get_intance_ru()
        {
            return ru::get_instance();
        }
    };
};