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

#include "object_uid.h"
#include "object_function_built_in.h"
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include "lib/functional.h"
#include <sstream>
#include <random>

namespace g0at
{
    namespace model
    {
        object_uid::object_uid(object_pool *pool)
            : object(pool, pool->get_uid_proto_instance())
        {
            reinit();
        }

        void object_uid::kill(object_pool *pool)
        {
            if (pool->uids.destroy_or_cache(this, pool))
            {
                objects.clear();
            }
        }
        
        void object_uid::reinit()
        {
            std::random_device rd;
            std::mt19937 gen(rd()); 
            std::uniform_int_distribution<> dis(0, 255);
            for (int i = 0 ; i < data.size; i++)
            {
                data.values[i] = (uint8_t)dis(gen);
            }
        }

        object_type object_uid::get_type() const
        {
            return object_type::uid;
        }

        object_uid *object_uid::to_object_uid()
        {
            return this;
        }

        bool object_uid::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::uid);
            const object_uid *uid = static_cast<const object_uid*>(obj);
            return data < uid->data;
        }

        std::wstring object_uid::to_string() const
        {
            std::wstringstream wss;
            for (int i = 0; i < 8; i++)
            {
                uint8_t value = data.values[i];
                uint8_t high = value >> 4;
                uint8_t low = value & 15;
                wss << (char)(high > 9 ? high - 10 + 'a' : high + '0');
                wss << (char)(low > 9 ? low - 10 + 'a' : low + '0');
            }
            return wss.str();
        }

        std::wstring object_uid::to_string_notation() const
        {
            return L"uid";
        }

        void object_uid::op_eq(thread *thr)
        {
            binary_logical_operation<lib::func::equals, true>(thr);
        }

        void object_uid::op_neq(thread *thr)
        {
            binary_logical_operation<lib::func::not_equal, false>(thr);
        }

        template <template<typename R, typename X, typename Y> class F, bool Def> void object_uid::binary_logical_operation(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            object *r_obj = right.get_object();
            variable result;
            do
            {
                if (r_obj)
                {
                    object_uid *r_uid = r_obj->to_object_uid();
                    if (r_uid)
                    {
                        result.set_boolean(F<bool, object_uid_data, object_uid_data>::calculate(data, r_uid->get_data()));
                        break;
                    }
                }
                result.set_boolean(Def);
            } while (false);
            thr->push(result);
        }

        /*
            Prototype
        */

        object_uid_proto::object_uid_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_uid_proto::init(object_pool *pool)
        {
            lock();
        }
    };
};
