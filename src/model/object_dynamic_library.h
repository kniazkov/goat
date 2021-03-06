/*

Copyright (C) 2021 Ivan Kniazkov

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
#include "lib/dll_loader.h"

namespace g0at
{
    namespace model
    {
        class object_dynamic_library : public object
        {
        public:
            object_dynamic_library(object_pool *_pool, lib::dll_loader *_loader);
            ~object_dynamic_library();
            object_dynamic_library *to_object_dynamic_library() override;
            std::wstring to_string() const override;
            
            lib::dll_loader *get_loader() { return loader; }

        protected:
            lib::dll_loader *loader;
        };

        class object_dynamic_library_proto : public object
        {
        friend class object_pool;
        protected:
            object_dynamic_library_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};