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

#include "rle.h"

namespace g0at
{
    namespace lib
    {
        void encode_rle(std::vector<uint8_t> &src, std::vector<uint8_t> &dst)
        {
            if (src.size() > 0)
            {
                uint8_t prev = src[0];
                int8_t rep = 1,
                    skip = 0;
                int left = 0,
                    index = 1,
                    max_index = (int)src.size();

                while (index < max_index)
                {
                    uint8_t ch = src[index];
                    if (ch != prev || rep > 126) {
                        if (rep > 2)
                        {
                            dst.push_back((uint8_t)rep);
                            dst.push_back(prev);
                        }
                        else
                        {
                            if (!skip)
                            {
                                left = index - rep;
                            }
                            skip += rep;
                        }
                        rep = 1;
                        prev = ch;
                    }
                    else
                    {
                        rep++;
                        if (rep == 3)
                        {
                            if (skip > 0)
                            {
                                dst.push_back((uint8_t)(-skip));
                                for (int8_t k = 0; k < skip; k++)
                                {
                                    dst.push_back(src[left + k]);
                                }
                                skip = 0;
                            }
                        }
                    }
                    index++;
                }

                if (rep > 0 && rep < 3)
                {
                    if (!skip)
                    {
                        left = index - rep;
                    }
                    skip += rep;
                    rep = 0;
                }
                if (skip > 0)
                {
                    dst.push_back((uint8_t)-skip);
                    for (int8_t k = 0; k < skip; k++)
                    {
                        dst.push_back(src[left + k]);
                    }
                }
                if (rep > 0)
                {
                    dst.push_back((uint8_t)rep);
                    dst.push_back(prev);
                }
            }
        }

        bool decode_rle(std::vector<uint8_t> &src, int start_index, std::vector<uint8_t> &dst)
        {
            int index = start_index,
                max_index = (int)src.size();

            while (index < max_index)
            {
                int8_t len = (int8_t)src[index++];
                if (len == 0)
                    return false;
                if (len > 0)
                {
                    if (index >= max_index)
                        return false;
                    uint8_t byte = src[index++];
                    for (int8_t k = 0; k < len; k++)
                    {
                        dst.push_back(byte);
                    }
                }
                else
                {
                    len = -len;
                    for (int8_t k = 0; k < len; k++)
                    {
                        if (index >= max_index)
                            return false;
                        uint8_t byte = src[index++];
                        dst.push_back(byte);
                    }
                }
            }
            return true;
        }
    };
};