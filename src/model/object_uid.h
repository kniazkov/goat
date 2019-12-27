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

#include "object.h"

namespace g0at
{
    namespace model
    {
        class object_uid_data
        {
        public:
            static const int size = 16;
            uint8_t values[size];

            object_uid_data()
            {
                for (int i = 0; i < size; i++)
                {
                    values[i] = 0;
                }
            }

            bool operator<(const object_uid_data &uid) const
            {
                for (int i = 0; i < size; i++)
                {
                    if (values[i] != uid.values[i])
                        return values[i] < uid.values[i];
                }
                return false;
            }

            bool operator==(const object_uid_data &uid) const
            {
                for (int i = 0; i < size; i++)
                {
                    if (values[i] != uid.values[i])
                        return false;
                }
                return true;
            }

            bool operator!=(const object_uid_data &uid) const
            {
                for (int i = 0; i < size; i++)
                {
                    if (values[i] != uid.values[i])
                        return true;
                }
                return false;
            }
        };

        class object_uid : public object
        {
        friend class object_pool;
        protected:
            object_uid(object_pool *pool);
            void reinit();
            
        public:
#ifndef GC_DEBUG
            void kill(object_pool *pool) override;
#endif            
            object_type get_type() const override;
            object_uid *to_object_uid() override;
            bool less(const object *obj) const override;
            std::wstring to_string() const override;
            std::wstring to_string_notation() const override;
            void op_eq(thread *thr) override;
            void op_neq(thread *thr) override;

            object_uid_data &get_data() { return data; }

        protected:
            object_uid_data data;

            template <template<typename R, typename X, typename Y> class F, bool Def> void binary_logical_operation(thread *thr);
        };

        class object_uid_proto : public object
        {
        friend class object_pool;
        protected:
            object_uid_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};