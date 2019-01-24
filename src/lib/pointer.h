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

namespace g0at
{
    namespace lib
    {
        template <typename T> class pointer
        {
        public:
            pointer()
                : obj(nullptr)
            {
            }

            pointer(T *_obj)
            {
                obj = _obj;
                if (obj)
                    obj->refs++;
            }

            pointer(const pointer &_ptr)
            {
                obj = _ptr.obj;
                if (obj)
                    obj->refs++;
            }

#if 0 /* it does not work in Windows */
            pointer(pointer &&_ptr)
            {
                T *tmp = obj;
                obj = _ptr.obj;
                _ptr.obj = tmp;
            }
#endif

            ~pointer()
            {
                if (obj && !(--obj->refs))
                    delete obj;
            }

            pointer &operator=(T *_obj)
            {
                if (obj != _obj)
                {
                    if (obj && !(--obj->refs))
                        delete obj;
                    obj = _obj;
                    if (obj)
                        obj->refs++;
                }
                return *this;
            }

            pointer &operator=(const pointer &_ptr)
            {
                if (obj != _ptr.obj)
                {
                    if (obj && !(--obj->refs))
                        delete obj;
                    obj = _ptr.obj;
                    if (obj)
                        obj->refs++;
                }
                return *this;
            }

            T *get()
            {
                return obj;
            }

            void reset()
            {
                if (obj && !(--obj->refs))
                    delete obj;
                obj = nullptr;
            }

            void swap(pointer &_ptr)
            {
                T *tmp = obj;
                obj = _ptr.obj;
                _ptr.obj = tmp;
            }

            T *operator->()
            {
                return obj;
            }

            operator bool()
            {
                return obj != nullptr;
            }

            bool operator==(T *_obj)
            {
                return obj == _obj;
            }

            bool operator!=(T *_obj)
            {
                return obj != _obj;
            }

            bool operator==(pointer &_ptr)
            {
                return obj == _ptr.obj;
            }

            bool operator!=(pointer &_ptr)
            {
                return obj != _ptr.obj;
            }

            template <typename C> pointer<C> cast()
            {
                return pointer<C>(static_cast<C*>(obj));
            }

        protected:
            T *obj;
        };
    };
};
