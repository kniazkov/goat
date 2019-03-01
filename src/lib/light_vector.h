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

#include "exception.h"
#include <algorithm>

namespace g0at
{
    namespace lib
    {
        template <typename T, int N> class light_vector
        {
        public:
            light_vector()
                : length(0), capacity(0), tail(nullptr)
            {
            }

            ~light_vector()
            {
                delete tail;
            }

            int size()
            {
                return length;
            }

            T at(int idx)
            {
                if (idx < 0 || idx >= length)
                    throw index_out_of_bounds();

                return idx < N ? head[idx] : tail[idx - N];
            }

            T operator[](int idx)
            {
                return at(idx);
            }

            void push_back(T item)
            {
                if (length < N)
                {
                    head[length++] = item;
                }
                else
                {
                    int tail_len = length - N;
                    if (tail_len == capacity)
                    {
                        if (tail_len)
                        {
                            capacity = capacity * 2;
                            T *tmp = new T[capacity];
                            std::swap_ranges(tail, tail + tail_len, tmp);
                            delete tail;
                            tail = tmp;
                        }
                        else
                        {
                            capacity = N;
                            tail = new T[capacity];
                        }
                    }
                    tail[tail_len] = item;
                    length++;
                }
            }

            class iterator
            {
            public:
                iterator(light_vector *_vector, int _idx)
                    : vector(_vector), idx(_idx)
                {
                }

                T operator*()
                {
                    return vector->at(idx);
                }

                bool operator==(iterator &iter)
                {
                    return idx == iter.idx && vector == iter.vector;
                }

                bool operator!=(iterator &iter)
                {
                    return idx != iter.idx || vector != iter.vector;
                }

                iterator &operator++()
                {
                    idx++;
                    return *this;
                }

                iterator operator++(int)
                {
                    iterator prev = *this;
                    idx++;
                    return prev;
                }

                iterator &operator--()
                {
                    idx--;
                    return *this;
                }

                iterator operator--(int)
                {
                    iterator prev = *this;
                    idx--;
                    return prev;
                }

            protected:
                light_vector *vector;
                int idx;
            };

            iterator begin()
            {
                return iterator(this, 0);
            }

            iterator end()
            {
                return iterator(this, length);
            }

        protected:
            light_vector(const light_vector& ) { }
            void operator=(const light_vector &) { }

            int length;
            int capacity;
            T head[N];
            T *tail;
        };
    };
};
