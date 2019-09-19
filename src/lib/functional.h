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

#include <cmath>

namespace g0at
{
    namespace lib
    {
        namespace func
        {
            template <typename R, typename A> struct unary
            {
                typedef R ret_type;
                typedef A arg_type;
            };

            template <typename R, typename A> struct pos : public unary <R, A>
            {
                static R calculate(const A &a)
                {
                    return a;
                }
            };

            template <typename R, typename A> struct neg : public unary <R, A>
            {
                static R calculate(const A &a)
                {
                    return -a;
                }
            };

            template <typename R, typename A> struct log_not : public unary <R, A>
            {
                static R calculate(const A &a)
                {
                    return !a;
                }
            };

            template <typename R, typename A> struct inv : public unary <R, A>
            {
                static R calculate(const A &a)
                {
                    return ~a;
                }
            };

            template <typename R, typename A> struct inc : public unary <R, A>
            {
                static R calculate(const A &a)
                {
                    return a + 1;
                }
            };

            template <typename R, typename A> struct dec : public unary <R, A>
            {
                static R calculate(const A &a)
                {
                    return a - 1;
                }
            };

            template <typename R, typename X, typename Y> struct binary
            {
                typedef R ret_type;
                typedef X left_type;
                typedef Y right_type;
            };

            template <typename R, typename X, typename Y> struct plus : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x + y;
                }
            };

            template <typename R, typename X, typename Y> struct minus : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x - y;
                }
            };

            template <typename R, typename X, typename Y> struct mul : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x * y;
                }
            };

            template <typename R, typename X, typename Y> struct div : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x / y;
                }
            };

            template <typename R, typename X, typename Y> struct mod : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x % y;
                }
            };

            template <typename R, typename X, typename Y> struct exp : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return (R)pow(x, y);
                }
            };

            template <typename R, typename X, typename Y> struct equals : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x == y;
                }
            };

            template <typename R, typename X, typename Y> struct not_equal : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x != y;
                }
            };

            template <typename R, typename X, typename Y> struct less : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x < y;
                }
            };

            template <typename R, typename X, typename Y> struct less_or_equal : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x <= y;
                }
            };

            template <typename R, typename X, typename Y> struct greater : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x > y;
                }
            };

            template <typename R, typename X, typename Y> struct greater_or_equal : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x >= y;
                }
            };

            template <typename R, typename X, typename Y> struct bitwise_and : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x & y;
                }
            };

            template <typename R, typename X, typename Y> struct bitwise_or : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x | y;
                }
            };

            template <typename R, typename X, typename Y> struct bitwise_xor : public binary <R, X, Y>
            {
                static R calculate(const X &x, const Y &y)
                {
                    return x ^ y;
                }
            };
        };
    };
};