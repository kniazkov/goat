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

#include "pattern.h"
#include "grammar_factory.h"
#include "common_exceptions.h"
#include "compiler/ast/keyword_lock.h"
#include "compiler/ast/identifier.h"
#include "lib/assert.h"
#include "resource/strings.h"

namespace g0at
{
    namespace parser
    {
        class method_lock : public pattern
        {
        public:
            method_lock(parser_data *_data)
                : pattern(&_data->lock_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_lock *kw = tok->to_keyword_lock();
                assert(kw != nullptr);

                do
                {
                    if (!kw->next)
                        break;

                    if (kw->next->to_colon() != nullptr)    // lock :
                        break;

                    ast::brackets_pair *bpair = kw->next->to_brackets_pair(); 
                    if (bpair && bpair->get_symbol() == '(')  // lock()
                        break;

                    if (kw->prev)
                    {
                        if (kw->prev->to_dot() != nullptr)  // .lock
                            break;
                    }

                    return false;
                } while (false);

                ast::identifier *ident = new ast::identifier(kw->get_fragment(), resource::str_lock);
                kw->replace(ident);
                data->identifiers.add(ident);

                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_method_lock()
        {
            return new method_lock(data);
        }
    };
};
