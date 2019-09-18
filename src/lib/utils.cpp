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

#include "utils.h"
#include "global/global.h"
#include <sstream>
#include <sstream>
#include <cfloat>
#include <cstring>
#include <fstream>
#include <streambuf>
#include <iostream>

/*

                                          $"   *.
              d$$$$$$$P"                  $    J
                  ^$.                     4r  "
                  d"b                    .db
                 P   $                  e" $
        ..ec.. ."     *.              zP   $.zec..
    .^        3*b.     *.           .P" .@"4F      "4
  ."         d"  ^b.    *c        .$"  d"   $         %
 /          P      $.    "c      d"   @     3r         3
4        .eE........$r===e$$$$eeP    J       *..        b
$       $$$$$       $   4$$$$$$$     F       d$$$.      4
$       $$$$$       $   4$$$$$$$     L       *$$$"      4
4         "      ""3P ===$$$$$$"     3                  P
 *                 $       """        b                J
  ".             .P                    %.             @
    %.         z*"                      ^%.        .r"
       "*==*""                             ^"*==*""

*/

namespace g0at
{
    namespace lib
    {
        std::string file_name_from_full_path(std::string path)
        {
            const char *ptr = path.c_str();
            const char *begin = ptr;
            while (*ptr)
            {
                if (*ptr == '\\' || *ptr == '/')
                    begin = ptr + 1;
                ptr++;
            }
            return std::string(begin);
        }

        std::wstring double_to_wstring(double value)
        {
            return double_to_wstring(value, 7);
        }

        std::wstring double_to_wstring(double value, int precision)
        {
            // really, there must be an easier way to do it (and this is NOT std::to_wstring())
            std::stringstream buff;
            bool neg = false;
            if (value < 0)
            {
                neg = true;
                value = -value;
            }
            int64_t factor = 1;
            if (precision > 7) precision = 7;
            for (int i = 0; i < precision; i++)
            {
                factor *= 10;
            }
            int64_t ip = (long int)value;
            double dfp = value * factor - ip * factor;
            int64_t fp = (long int)dfp;
            if (fp > 0)
            {
                bool flag = false;
                while(precision > 0)
                {
                    char v = fp % 10;
                    if (v != 0)
                        flag = true;
                    if (flag)
                    {
                        char c = v + '0';
                        buff << c;
                    }
                    fp = fp / 10;
                    precision--;
                };
                buff << '.';
            }
            do
            {
                char c = ip % 10 + '0';
                buff << c;
                ip = ip / 10;
            } while(ip != 0);
            if (neg)
            {
                buff << '-';
            }
            std::string str = buff.str();
            std::wstring result;
            result.reserve(str.length());
            result.assign(str.rbegin(), str.rend());
            return result;
        }

        std::wstring escape_special_chars(std::wstring src)
        {
            return escape_special_chars(src.c_str(), src.length());
        }

        std::wstring escape_special_chars(const wchar_t *src, size_t len)
        {
            std::wstringstream dst;
            for (size_t i = 0; i < len; i++)
            {
                wchar_t c = src[i];
                switch(c)
                {
                    case L'\n':
                        dst << L"\\n"; break;
                    case L'\r':
                        dst << L"\\r"; break;
                    case L'\t':
                        dst << L"\\t"; break;
                    case L'\'':
                        dst << L"\\\'"; break;
                    case L'\"':
                        dst << L"\\\""; break;
                    case L'\\':
                        dst << L"\\\\"; break;
                    default:
                        dst << c;
                }
            }
            return dst.str();
        }

        std::wstring escape_html_entities(std::wstring src)
        {
            return escape_html_entities(src.c_str(), src.length());
        }

        std::wstring escape_html_entities(const wchar_t *src, size_t len)
        {
            std::wstringstream dst;
            for (size_t i = 0; i < len; i++)
            {
                wchar_t c = src[i];
                switch(c)
                {
                    case L'<':
                        dst << L"&lt;"; break;
                    case L'>':
                        dst << L"&gt;"; break;
                    case L'&':
                        dst << L"&amp;"; break;
                    case L'[':
                        dst << L"&#91;"; break;
                    case L']':
                        dst << L"&#93;"; break;
                    default:
                        dst << c;
                }
            }
            return dst.str();
        }

        char * file_name_postfix(const char *head, const char *tail)
        {
            auto head_len = std::strlen(head);
            auto tail_len = std::strlen(tail);
            char *dst = new char[head_len + tail_len + 2];
            std::memcpy(dst, head, head_len);
            dst[head_len] = '.';
            std::memcpy(dst + head_len + 1, tail, tail_len);
            dst[head_len + 1 + tail_len] = 0;
            return dst;
        }

        void dump_file(const char *file_name, const char *postfix, std::wstring data)
        {
            std::string tmp = global::char_encoder->encode(data);
            char *full_file_name = file_name_postfix(file_name, postfix);
            std::ofstream file(full_file_name);
            file.write(tmp.c_str(), tmp.size());
            file.close();
            delete full_file_name;
        }

        bool file_exists(const char *file_name)
        {
            std::ifstream stream(file_name);
            return stream.good();
        }

        std::string wstring_to_ascii_string(std::wstring src, bool *okay)
        {
            size_t len = src.length();
            std::string dst(len, '\0');
            if (okay)
            {
                *okay = true;
                for (size_t i = 0; i < len; i++)
                {
                    wchar_t ch = src[i];
                    if (ch < 128)
                    {
                        dst[i] = (char)ch;
                    }
                    else
                    {
                        *okay = false;
                        break;
                    }       
                }
            }
            else
            {
                for (size_t i = 0; i < len; i++)
                {
                    wchar_t ch = src[i];
                    dst[i] = ch < 128 ? (char)ch : '?';
                }
            }
            return dst;
        }

        std::string normalize_file_path(std::string path)
        {
#ifdef _WIN32
            const char separator = '\\';
            const char wrong_separator = '/';
#else
            const char separator = '/';
            const char wrong_separator = '\\';
#endif
            std::stringstream stream;
            bool flag = false;
            for (size_t i = 0, len = path.length(); i < len; i++)
            {
                char c = path[i];
                if (c == separator || c == wrong_separator)
                {
                    if (!flag)
                    {
                        flag = true;
                      stream << separator;
                    }
                }
                else
                {
                    stream << c;
                    flag = false;
                }
            }
            return stream.str();
        }

        std::string trim(std::string str)
        {
            size_t size = str.size();
            if (size == 0)
                return str;

            size_t begin = 0;
            while(begin < size)
            {
                if (str[begin] != ' ')
                    break;
                begin++;
            }

            if (begin == size)
                return "";
            
            size_t end = size - 1;
            while(end > begin)
            {
                if (str[end] != ' ')
                    break;
                end--;
            }

            return str.substr(begin, end - begin + 1);
        }

        std::string get_line()
        {
            std::string line;
            std::getline(std::cin, line);
            return trim(line);
        }
    };
};