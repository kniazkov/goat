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
#include "compiler/common/compilation_error.h"
#include "global/global.h"
#include <sstream>
#include <cstdint>
#include "compiler/ast/identifier.h"
#include "compiler/ast/bracket.h"
#include "compiler/ast/static_string.h"
#include "compiler/ast/semicolon.h"
#include "compiler/ast/plus.h"
#include "compiler/ast/custom_operator.h"
#include "compiler/ast/integer.h"
#include "compiler/ast/minus.h"
#include "compiler/ast/value_void.h"
#include "compiler/ast/value_undefined.h"
#include "compiler/ast/value_null.h"
#include "compiler/ast/keyword_var.h"
#include "compiler/ast/assign.h"
#include "compiler/ast/comma.h"
#include "compiler/ast/real.h"
#include "compiler/ast/keyword_function.h"
#include "compiler/ast/keyword_return.h"
#include "compiler/ast/colon.h"
#include "compiler/ast/dot.h"
#include "compiler/ast/value_true.h"
#include "compiler/ast/value_false.h"
#include "compiler/ast/equals.h"
#include "compiler/ast/not_equal.h"
#include "compiler/ast/keyword_while.h"
#include "compiler/ast/this_ptr.h"
#include "compiler/ast/keyword_if.h"
#include "compiler/ast/keyword_else.h"
#include "compiler/ast/keyword_throw.h"
#include "compiler/ast/keyword_try.h"
#include "compiler/ast/keyword_catch.h"
#include "compiler/ast/keyword_finally.h"
#include "compiler/ast/inherit.h"
#include "compiler/ast/character.h"
#include "compiler/ast/keyword_for.h"
#include "compiler/ast/keyword_new.h"
#include "compiler/ast/less.h"
#include "compiler/ast/increment.h"
#include "compiler/ast/decrement.h"
#include "compiler/ast/keyword_thread.h"
#include "compiler/ast/keyword_lock.h"
#include "compiler/ast/keyword_in.h"
#include "compiler/ast/keyword_do.h"
#include "compiler/ast/keyword_break.h"
#include "compiler/ast/keyword_continue.h"
#include "compiler/ast/keyword_switch.h"
#include "compiler/ast/keyword_case.h"
#include "compiler/ast/keyword_default.h"
#include "compiler/ast/keyword_import.h"
#include "compiler/ast/asterisk.h"
#include "compiler/ast/double_asterisk.h"
#include "compiler/ast/slash.h"
#include "compiler/ast/percent.h"
#include "compiler/ast/exclamation.h"
#include "compiler/ast/tilde.h"
#include "compiler/ast/double_exclamation.h"

namespace g0at
{
    class unknown_symbol : public compilation_error
    {
    public:
        unknown_symbol(lib::pointer<position> pos, wchar_t ch)
            : compilation_error(pos, global::resource->unknown_character(ch))
        {
        }
    };

    class missing_closing_quote : public compilation_error
    {
    public:
        missing_closing_quote(lib::pointer<position> pos)
            : compilation_error(pos, global::resource->missing_closing_quote())
        {
        }
    };

    class invalid_escape_sequence : public compilation_error
    {
    public:
        invalid_escape_sequence(lib::pointer<position> pos, wchar_t ch)
            : compilation_error(pos, global::resource->invalid_escape_sequence(ch))
        {
        }
    };

    class unclosed_comment : public compilation_error
    {
    public:
        unclosed_comment(lib::pointer<position> pos)
            : compilation_error(pos, global::resource->unclosed_comment())
        {
        }
    };

    class character_constant_too_long_for_its_type : public compilation_error
    {
    public:
        character_constant_too_long_for_its_type(lib::pointer<position> pos)
            : compilation_error(pos, global::resource->character_constant_too_long_for_its_type())
        {
        }
    };

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

    static bool is_operator(wchar_t c)
    {
        switch(c)
        {
            case L'+':
            case L'-':
            case L'*':
            case L'/':
            case L'=':
            case L'$':
            case L'!':
            case L'<':
            case L'>':
            case L'%':
            case L'~':
                return true;
            default:
                return false;
        }
    }

    std::wstring scanner::parse_string_sequence(wchar_t closing_quote)
    {
        std::wstringstream wss;
        wchar_t c = src->next();
        while(c != closing_quote)
        {
            if (c == L'\0')
            {
                throw missing_closing_quote(src->get_position());
            }
            if (c == L'\\')
            {
                wchar_t e = src->next();
                switch(e)
                {
                    case L'n':
                        wss << L'\n'; break;
                    case L'r':
                        wss << L'\r'; break;
                    case L't':
                        wss << L'\t'; break;
                    case L'\'':
                        wss << L'\''; break;
                    case L'\"':
                        wss << L'\"'; break;
                    case L'\\':
                        wss << L'\\'; break;
                    default:
                        throw invalid_escape_sequence(src->get_position(), e);
                }
            }
            else
            {
                wss << c;
            }
            c = src->next();
        }
        src->next();
        return wss.str();
    }

    lib::pointer<ast::token> scanner::get_token()
    {
        wchar_t c = src->get_char();

        while(is_space(c))
        {
            c = src->next();
        }

        lib::pointer<position> pos = src->get_position();
        lib::pointer<ast::token> tok = create_token();
        if (tok)
            tok->pos = pos;
        return tok;
    }

    lib::pointer<ast::token> scanner::create_token()
    {
        wchar_t c = src->get_char();


        while(true)
        {
            if (c == L'/')
            {
                wchar_t c_1 = src->get_char(1);
                if (c_1 == L'/')
                {
                    src->next();
                    c = src->next();
                    while (c != L'\n' && c != '\0')
                    {
                        c = src->next();
                    }
                    while(is_space(c))
                    {
                        c = src->next();
                    }
                }
                else if (c_1 == L'*')
                {
                    src->next();
                    while (true)
                    {
                        c = src->next();
                        if (c == '\0')
                        {
                            throw unclosed_comment(src->get_position());
                        }
                        if (c == L'*')
                        {
                            if (src->get_char(1) == L'/')
                            {
                                src->next();
                                c = src->next();
                                break;
                            }
                        }
                    }
                    while(is_space(c))
                    {
                        c = src->next();
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        if (is_letter(c))
        {
            std::wstringstream wss;
            do
            {
                wss << c;
                c = src->next();
            } while(is_letter(c) || is_digit(c));
            std::wstring name = wss.str();
            if (name == L"void")
                return new ast::value_void();            
            if (name == L"undefined")
                return new ast::value_undefined();            
            if (name == L"null")
                return new ast::value_null();            
            if (name == L"true")
                return new ast::value_true();            
            if (name == L"false")
                return new ast::value_false();            
            if (name == L"this")
                return new ast::this_ptr();            
            if (name == L"var")
                return new ast::keyword_var();            
            if (name == L"function")
                return new ast::keyword_function();            
            if (name == L"return")
                return new ast::keyword_return();
            if (name == L"throw")
                return new ast::keyword_throw();            
            if (name == L"while")
                return new ast::keyword_while();            
            if (name == L"if")
                return new ast::keyword_if();            
            if (name == L"else")
                return new ast::keyword_else();            
            if (name == L"try")
                return new ast::keyword_try();            
            if (name == L"catch")
                return new ast::keyword_catch();            
            if (name == L"finally")
                return new ast::keyword_finally();            
            if (name == L"for")
                return new ast::keyword_for();            
            if (name == L"new")
                return new ast::keyword_new();            
            if (name == L"thread")
                return new ast::keyword_thread();            
            if (name == L"lock")
                return new ast::keyword_lock();
            if (name == L"in")
                return new ast::keyword_in();
            if (name == L"do")
                return new ast::keyword_do();
            if (name == L"break")
                return new ast::keyword_break();
            if (name == L"continue")
                return new ast::keyword_continue();
            if (name == L"switch")
                return new ast::keyword_switch();
            if (name == L"case")
                return new ast::keyword_case();
            if (name == L"default")
                return new ast::keyword_default();
            if (name == L"import")
                return new ast::keyword_import();
            return new ast::identifier(name);
        }

        if (c == L'\"')
        {
            std::wstring str = parse_string_sequence('\"');
            return new ast::static_string(str);
        }

        if (c == L'\'')
        {
            std::wstring str = parse_string_sequence('\'');
            if (str.length() != 1)
                throw character_constant_too_long_for_its_type(src->get_position());
            return new ast::character(str[0]);
        }

        if (is_digit(c))
        {
            int64_t int_val = 0;
            do
            {
                int_val = int_val * 10 + c - L'0';
                c = src->next();
            } while(is_digit(c));
            if (c != L'.')
                return new ast::integer(int_val);
            c = src->next();
            if (!is_digit(c))
                return new ast::integer(int_val);
            int64_t fract_val = 0;
            int64_t divisor = 1;
            do
            {
                fract_val = fract_val * 10 + c - L'0';
                divisor *= 10;
                c = src->next();
            } while(is_digit(c));
            return new ast::real((double)int_val + (double)fract_val / (double)divisor);
        }

        if (is_operator(c))
        {
            std::wstringstream wss;
            do
            {
                wss << c;
                c = src->next();
            } while(is_operator(c));
            std::wstring oper = wss.str();
            if (oper == L"+")
                return new ast::plus();
            if (oper == L"-")
                return new ast::minus();
            if (oper == L"*")
                return new ast::asterisk();
            if (oper == L"**")
                return new ast::double_asterisk();
            if (oper == L"/")
                return new ast::slash();
            if (oper == L"%")
                return new ast::percent();
            if (oper == L"=")
                return new ast::assign();
            if (oper == L"==")
                return new ast::equals();
            if (oper == L"!=")
                return new ast::not_equal();
            if (oper == L"<")
                return new ast::less();
            if (oper == L"->")
                return new ast::inherit();
            if (oper == L"++")
                return new ast::increment();
            if (oper == L"--")
                return new ast::decrement();
            if (oper == L"!")
                return new ast::exclamation();
            if (oper == L"!!")
                return new ast::double_exclamation();
            if (oper == L"~")
                return new ast::tilde();
            if (oper == L"$")
                return new ast::keyword_function();
            if (oper == L"$$")
                return new ast::keyword_thread();
            return new ast::custom_operator(oper);
        }
        
        if (c == L'(')
        {
            src->next();
            return new ast::bracket(L'(', L')', false);
        }

        if (c == L'{')
        {
            src->next();
            return new ast::bracket(L'{', L'}', false);
        }

        if (c == L'[')
        {
            src->next();
            return new ast::bracket(L'[', L']', false);
        }

        
        if (c == L')')
        {
            src->next();
            return new ast::bracket(L')', L'(', true);
        }

        if (c == L'}')
        {
            src->next();
            return new ast::bracket(L'}', L'{', true);
        }

        if (c == L']')
        {
            src->next();
            return new ast::bracket(L']', L'[', true);
        }

        if (c == L';')
        {
            src->next();
            return new ast::semicolon();
        }

        if (c == L',')
        {
            src->next();
            return new ast::comma();
        }

        if (c == L':')
        {
            src->next();
            return new ast::colon();
        }

        if (c == L'.')
        {
            src->next();
            return new ast::dot();
        }

        if (c == L'\0')
        {
            return nullptr;
        }

        throw unknown_symbol(src->get_position(), c);
    }
};
