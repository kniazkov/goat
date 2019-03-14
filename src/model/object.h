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

#define MODEL_DEBUG

#include "object_pool.h"
#include "lib/light_vector.h"
#include <map>
#include <vector>
#include <string>

namespace g0at
{
    namespace model
    {
        class thread;
        class object;
        class object_string;
        class object_function;
        class object_integer;
        class object_real;
        class object_void;
        class object_undefined;
        class object_null;
        class object_boolean;
        class handler;

        enum class object_type
        {
            generic,
            string,
            function,
            integer,
            real,
            boolean
        };

        class object_comparator
        {
        public:
            inline bool operator() (const object *a, const object *b) const;
        };

        class variable
        {
        public:
            variable()
            {
                hndl = nullptr;
                data.obj = nullptr;
            }

            inline void set_object(object *obj);
            inline void set_integer(int64_t value);
            inline void set_real(double value);
            inline void set_boolean(bool value);

            inline std::wstring to_string() const;
            inline std::wstring to_string_notation() const;
            inline object *to_object(object_pool *pool);

            inline object *get_object();
            inline void mark();
            inline bool get_integer(int64_t *pval);
            inline bool get_real(double *pval);
            inline bool get_boolean(bool *pval);

            inline void op_add(thread *thr);
            inline void op_sub(thread *thr);
            inline void op_neg(thread *thr);
            inline void op_eq(thread *thr);
            inline void op_neq(thread *thr);

            inline void m_clone(thread *thr, int arg_count);

            handler *hndl;
            union
            {
                object *obj;
                int64_t i;
                double r;
                bool b;
            } data;
        };

        class object
        {
        public:
            object(object_pool *pool);
            object(object_pool *pool, object *proto);
            object(object_pool *pool, object *proto_1, object *proto_2);
            virtual ~object();
#ifdef MODEL_DEBUG
            int get_id() { return id; }
#else
            int get_id() { return 0; }
#endif
            virtual void kill(object_pool *pool);
            virtual void trace();
            inline void mark();
            inline void unmark();
            inline void sweep(object_pool *pool);

            virtual object_type get_type() const;
            virtual object_string *to_object_string();
            virtual object_function *to_object_function();
            virtual object_integer *to_object_integer();
            virtual object_real *to_object_real();
            virtual object_void *to_object_void();
            virtual object_undefined *to_object_undefined();
            virtual object_null *to_object_null();
            virtual object_boolean *to_object_boolean();

            virtual bool less(const object *obj) const;
            virtual std::wstring to_string() const;
            virtual std::wstring to_string_notation() const;
            void copy_objects_to(object *dst);
            void copy_proto_to(object *dst);

            void add_object(object *key, variable &value);
            void add_object(object *key, object *value);
            variable *find_object(object *key);

            virtual bool get_integer(int64_t *pval);
            virtual bool get_real(double *pval);
            virtual bool get_boolean(bool *pval);

            virtual void op_add(thread *thr);
            virtual void op_sub(thread *thr);
            virtual void op_neg(thread *thr);
            virtual void op_eq(thread *thr);
            virtual void op_neq(thread *thr);

            virtual void m_clone(thread *thr, int arg_count);

            object *prev;
            object *next;

        protected:
#ifdef MODEL_DEBUG
            int id;
#endif
            bool marked;
            std::map<object*, variable, object_comparator> objects;
#if 0
            std::vector<object*> proto;
#else
            lib::light_vector<object*, 2> proto;
#endif
        };

        class generic_object : public object
        {
        friend class object_pool;
        public:
            void kill(object_pool *pool) override;
        protected:
            generic_object(object_pool *pool);
            void reinit(object_pool *pool);
        public:
            virtual void m_clone(thread *thr, int arg_count) override;
        };

        class generic_proto : public object
        {
        friend class object_pool;
        protected:
            generic_proto(object_pool *pool);
            void init(object_pool *pool);
        };

        class handler
        {
        public:
            virtual ~handler();
            static handler *get_instance_generic();
            static handler *get_instance_integer();
            static handler *get_instance_real();
            static handler *get_instance_boolean();
            virtual std::wstring to_string(const variable *var) const = 0;
            virtual std::wstring to_string_notation(const variable *var) const = 0;
            virtual object *to_object(variable *var, object_pool *pool) = 0;
            
            virtual object* get_object(variable *var);
            virtual bool get_integer(variable *var, int64_t *val);
            virtual bool get_real(variable *var, double *val);
            virtual bool get_boolean(variable *var, bool *val);

            virtual void op_add(variable *var, thread *thr);
            virtual void op_sub(variable *var, thread *thr);
            virtual void op_neg(variable *var, thread *thr);
            virtual void op_eq(variable *var, thread *thr);
            virtual void op_neq(variable *var, thread *thr);

            virtual void m_clone(variable *var, thread *thr, int arg_count);
        };

        bool object_comparator::operator ()(const object *a, const object *b) const
        {
            if (a == b)
                return false;

            object_type x = a->get_type();
            object_type y = b->get_type();

            if (x == y)
                return a->less(b);

            return x < y;
        }

        /*
            Variable inline methods
        */

        void variable::set_object(object *obj)
        {
            hndl = handler::get_instance_generic();
            data.obj = obj;
        }

        void variable::set_integer(int64_t value)
        {
            hndl = handler::get_instance_integer();
            data.i = value;
        }

        void variable::set_real(double value)
        {
            hndl = handler::get_instance_real();
            data.r = value;
        }

        void variable::set_boolean(bool value)
        {
            hndl = handler::get_instance_boolean();
            data.b = value;
        }

        std::wstring variable::to_string() const
        {
            return hndl->to_string(this);
        }

        std::wstring variable::to_string_notation() const
        {
            return hndl->to_string_notation(this);
        }

        object *variable::to_object(object_pool *pool)
        {
            return hndl->to_object(this, pool);
        }

        object *variable::get_object()
        {
            return hndl->get_object(this);
        }

        void variable::mark()
        {
            object *obj = hndl->get_object(this);
            if (obj)
            {
                obj->mark();
            }
        }

        bool variable::get_integer(int64_t *pval)
        {
            return hndl->get_integer(this, pval);
        }

        bool variable::get_real(double *pval)
        {
            return hndl->get_real(this, pval);
        }

        bool variable::get_boolean(bool *pval)
        {
            return hndl->get_boolean(this, pval);
        }

        void variable::op_add(thread *thr)
        {
            hndl->op_add(this, thr);
        }

        void variable::op_sub(thread *thr)
        {
            hndl->op_sub(this, thr);
        }

        void variable::op_neg(thread *thr)
        {
            hndl->op_neg(this, thr);
        }

        void variable::op_eq(thread *thr)
        {
            hndl->op_eq(this, thr);
        }

        void variable::op_neq(thread *thr)
        {
            hndl->op_neq(this, thr);
        }
        
        void variable::m_clone(thread *thr, int arg_count)
        {
            hndl->m_clone(this, thr, arg_count);
        }


        /*
            Object inline methods
        */
        void object::mark()
        {
            if (!marked)
            {
                marked = true;

                for (auto pair : objects)
                {
                    pair.first->mark();
                    pair.second.mark();
                }

                for (object *pr : proto)
                {
                    pr->mark();
                }

                trace();
            }
        }

        void object::unmark()
        {
            marked = false;
        }

        void object::sweep(object_pool *pool)
        {
            if (!marked)
            {
                kill(pool);
            }
            else
            {
                unmark();
            }
        }
    };
};
