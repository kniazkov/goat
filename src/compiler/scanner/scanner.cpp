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

#include "scanner.h"
#include "../ast/identifier.h"
#include "../ast/bracket.h"
#include "../ast/static_string.h"
#include "../ast/semicolon.h"
#include <memory>
#include <sstream>

namespace g0at
{
    scanner::scanner(source *_src)
        : src(_src)
    {
    }

    static inline bool is_space(wchar_t c)
    {
        return c == L' ' || c == L'\t' || c == L'\n' || c == L'\r';
    }

    static inline bool is_letter(wchar_t c)
    {
        return (c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') || c == L'_' || c > 128;
    }

    static inline bool is_digit(wchar_t c)
    {
        return c >= L'0' && c <= L'9';
    }

    std::shared_ptr<ast::token> scanner::get_token()
    {
        wchar_t c = src->get_char();

        while(is_space(c))
        {
            c = src->next();
        }

        std::shared_ptr<position> pos = src->get_position();
        std::shared_ptr<ast::token> tok = create_token();
        if (tok)
            tok->pos = pos;
        return tok;
    }

    std::shared_ptr<ast::token> scanner::create_token()
    {
        wchar_t c = src->get_char();

        if (is_letter(c))
        {
            std::wstringstream wss;
            do
            {
                wss << c;
                c = src->next();
            } while(is_letter(c) || is_digit(c));
            
            return std::make_shared<ast::identifier>(wss.str());
        }

        if (c == L'"')
        {
            std::wstringstream wss;
            c = src->next();
            while(c != L'"')
            {
                wss << c;
                c = src->next();
            }
            src->next();
            return std::make_shared<ast::static_string>(wss.str());
        }
        
        if (c == L'(')
        {
            src->next();
            return std::make_shared<ast::bracket>(L'(', L')', false);
        }

        if (c == L'{')
        {
            src->next();
            return std::make_shared<ast::bracket>(L'{', L'}', false);
        }

        if (c == L'[')
        {
            src->next();
            return std::make_shared<ast::bracket>(L'[', L']', false);
        }

        
        if (c == L')')
        {
            src->next();
            return std::make_shared<ast::bracket>(L')', L'(', true);
        }

        if (c == L'}')
        {
            src->next();
            return std::make_shared<ast::bracket>(L'}', L'{', true);
        }

        if (c == L']')
        {
            src->next();
            return std::make_shared<ast::bracket>(L']', L'[', true);
        }

        if (c == L';')
        {
            src->next();
            return std::make_shared<ast::semicolon>();
        }

        return nullptr;
    }
};
