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
        enum class file_access_mode
        {
            closed = 0,
            read,
            write,
            append,
            full
        };

        struct file_descriptor
        {
            void *sysctl;
            file_access_mode mode;
        };

        class object_file : public object
        {
        public:
            object_file(object_pool *_pool, file_descriptor _descr);
            ~object_file();
            object_file *to_object_file() override;
            std::wstring to_string() const override;
            
            file_descriptor *get_descriptor() { return &descr; }

        protected:
            file_descriptor descr;
        };

        class object_file_proto : public object
        {
        friend class object_pool;
        protected:
            object_file_proto(object_pool *pool);
            void init(object_pool *pool);
        };
    };
};