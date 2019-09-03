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

#include "utf8_encoder.h"
#include "global/global.h"
#include <cstdint>
#include <sstream>
#include "lib/assert.h"

namespace g0at
{
    namespace lib
    {
        class bad_utf8 : public exception
        {
        public:
            bad_utf8()
                : exception(global::resource->bad_utf8())
            {
            }
        };

        static int encode_utf8_char(wchar_t w, char *c)
        {
            if (w < 0x80)
            {
                c[0] = (char)w;
                return 1;
            }
            if (w < 0x800)
            {
                c[0] = (char)((w & 0x7C0) >> 6) + 0xC0;
                c[1] = (char)(w & 0x3F) + 0x80;
                return 2;
            }
            if (w < 0x10000)
            {
                c[0] = (char)((w & 0xF000) >> 12) + 0xE0;
                c[1] = (char)((w & 0xFC0) >> 6) + 0x80;
                c[2] = (char)(w & 0x3F) + 0x80;
                return 3;
            }
#if WCHAR_MAX > 0xFFFF
            if (w < 0x200000)
            {
                c[0] = (char)((w & 0x1C0000) >> 18) + 0xF0;
                c[1] = (char)((w & 0x3F000) >> 12) + 0x80;
                c[2] = (char)((w & 0xFC0) >> 6) + 0x80;
                c[3] = (char)(w & 0x3F) + 0x80;
                return 3;
            }
#endif
		    return 0;
        }

        utf8_encoder::utf8_encoder()
        {
        }

        char_encoder *utf8_encoder::get_instance()
        {
            static utf8_encoder instance;
            return &instance;
        }

        std::string utf8_encoder::encode(std::wstring wstr)
        {
            return encode_utf8(wstr);
        }

        std::string encode_utf8(std::wstring wstr)
        {
            std::stringstream ss;
            int i, n;
            char tmp[4];
            for (wchar_t w : wstr)
            {
                n = encode_utf8_char(w, tmp);
                assert(n > 0);
                for (i = 0; i < n; i++)
                    ss << tmp[i];
            }
            return ss.str();
        }

        std::wstring utf8_encoder::decode(std::string str)
        {
            return decode_utf8(str);
        }

        std::wstring decode_utf8(std::string str)
        {
            size_t i = 0,
                len = str.size();
            std::wstringstream wss;
            while (i < len)
            {
                unsigned char c0 = (unsigned char)str[i];
                wchar_t w = 0;
                if ((c0 & 0x80) == 0)
                {
                    w = c0;
                    i++;
                }
                else if ((c0 & 0xE0) == 0xC0)
                {
                    if (i + 1 >= len)
                    {
                        throw bad_utf8();
                    }
                    unsigned char c1 = (unsigned char)str[i + 1];
                    if ((c1 & 0xC0) != 0x80)
                    {
                        throw bad_utf8();
                    }
                    w = ((c0 & 0x1F) << 6) + (c1 & 0x3F);
                    i += 2;
                }
                else if ((c0 & 0xF0) == 0xE0)
                {
                    if (i + 2 >= len)
                    {
                        throw bad_utf8();
                    }
                    unsigned char c1 = (unsigned char)str[i + 1];
                    unsigned char c2 = (unsigned char)str[i + 2];
                    if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80)
                    {
                        throw bad_utf8();
                    }
                    w = ((c0 & 0xF) << 12) + ((c1 & 0x3F) << 6) + (c2 & 0x3F);
                    i += 3;
                }
#if WCHAR_MAX > 0xFFFF
                else if ((c0 & 0xF8) == 0xF0)
                {
                    if (i + 3 >= len)
                    {
                        throw bad_utf8();
                    }
                    unsigned char c1 = (unsigned char)str[i + 1];
                    unsigned char c2 = (unsigned char)str[i + 2];
                    unsigned char c3 = (unsigned char)str[i + 3];
                    if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80)
                    {
                        throw bad_utf8();
                    }
                    w = ((c0 & 0x7) << 18) + ((c1 & 0x3F) << 12) + ((c2 & 0x3F) << 6) + (c3 & 0x3F);
                    i += 4;
                }
#endif
                else
                {
                    throw bad_utf8();
                }
                wss << w;
            }
            return wss.str();
        }
    };
};