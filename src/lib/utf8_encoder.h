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

#include "char_encoder.h"
#include "exception.h"

namespace g0at
{
    namespace lib
    {
        class utf8_encoder : public char_encoder
        {
        public:
            static char_encoder *get_instance();
            std::string encode(std::wstring wstr) override;
            std::wstring decode(std::string str) override;

        protected:
            utf8_encoder();
            static int encode_char(wchar_t w, char *c);
        };
    };
};