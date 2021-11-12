/*

Copyright (C) 2017-2021 Ivan Kniazkov

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

#include "function.h"
#include "keyword_function.h"
#include "keyword_thread.h"
#include "lib/assert.h"

namespace g0at
{
    namespace ast
    {
        function::function()
        {
        }

        function::function(keyword *_kw, brackets_pair *_args, brackets_pair *_body, function_type _type)
        {
            assert((_kw->to_keyword_function() != nullptr && _type == function_type::function) || (_kw->to_keyword_thread() != nullptr && _type == function_type::thread));
            frag.begin = _kw->get_fragment().begin;
            frag.end = _kw->get_fragment().begin;
            if (_args != nullptr)
            {
                assert(_args->get_symbol() == L'(');
                frag.end = _args->get_fragment().end;
                args_raw.swap(_args->get_raw_list());
            }
            if (_body != nullptr)
            {
                assert(_body->get_symbol() == L'{');
                frag.end = _body->get_fragment().end;
                raw.swap(_body->get_raw_list());
            }
            type = _type;
        }

        void function::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        function *function::to_function()
        {
            return this;
        }
    };
};
