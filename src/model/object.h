/*

Copyright (C) 2017-2020 Ivan Kniazkov

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

#include "settings.h"
#include "object_pool.h"
#include "lib/pointer.h"
#include "lib/ref_counter.h"
#include "lib/buffer.h"
#include <map>
#include <set>
#include <stack>
#include <string>
#include <functional>

namespace g0at
{
    namespace model
    {
        class thread;
        class object;
        class object_string;
        class object_function;
        class object_thread;
        class object_integer;
        class object_real;
        class object_void;
        class object_undefined;
        class object_null;
        class object_boolean;
        class object_array_like;
        class object_array;
        class object_exception;
        class object_char;
        class object_runner;
        class object_uid;
        class object_string_builder;
        class object_byte_array;
        class object_file;
        class object_port;
        class handler;
        enum class call_mode;

        enum class object_type
        {
            generic,
            string,
            function,
            integer,
            real,
            boolean,
            charact,
            uid
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
            inline void set_reference(variable *ref);
            inline void set_integer(int64_t value);
            inline void set_real(double value);
            inline void set_boolean(bool value);
            inline void set_char(wchar_t value);

            inline variable deref();

            inline std::wstring to_string() const;
            inline std::wstring to_string_notation() const;
            inline object *to_object(object_pool *pool);
            inline object *get_object();
            inline void mark();
            inline void mark_parallel(object_pool *pool);
            inline void put_off(object_pool *pool);
            inline bool get_integer(int64_t *pval);
            inline bool get_real(double *pval);
            inline bool get_boolean(bool *pval);
            inline bool get_char(wchar_t *pval);
            inline bool get_byte(uint8_t *pval);

            inline void op_add(thread *thr);
            inline void op_sub(thread *thr);
            inline void op_pos(thread *thr);
            inline void op_neg(thread *thr);
            inline void op_inc(thread *thr);
            inline void op_dec(thread *thr);
            inline void op_not(thread *thr);
            inline void op_bool(thread *thr);
            inline void op_inv(thread *thr);
            inline void op_mul(thread *thr);
            inline void op_exp(thread *thr);
            inline void op_div(thread *thr);
            inline void op_mod(thread *thr);
            inline void op_eq(thread *thr);
            inline void op_neq(thread *thr);
            inline void op_shl(thread *thr);
            inline void op_shr(thread *thr);
            inline void op_shrz(thread *thr);
            inline void op_less(thread *thr);
            inline void op_leq(thread *thr);
            inline void op_great(thread *thr);
            inline void op_greq(thread *thr);
            inline void op_inherit(thread *thr);
            inline void op_and(thread *thr);
            inline void op_or(thread *thr);
            inline void op_bitand(thread *thr);
            inline void op_bitor(thread *thr);
            inline void op_xor(thread *thr);
            inline void op_protect(thread *thr);

            inline void m_clone(thread *thr, int arg_count);
            inline void m_instance_of(thread *thr, int arg_count);
            inline void m_flat(thread *thr, int arg_count);
            inline void m_iterator(thread *thr, int arg_count);
            inline void m_next(thread *thr, int arg_count);
            inline void m_valid(thread *thr, int arg_count);

            handler *hndl;
            union
            {
                object *obj;
                variable *ref;
                int64_t i;
                double r;
                bool b;
                wchar_t c;
            } data;
        };

        class topology_descriptor : public lib::ref_counter
        {
        public:
            lib::buffer<object*> proto;
            lib::buffer<object*> flat;

            void build();
        };

        class object
        {
        friend class object_array;
        public:
            struct tsort_data
            {
                std::stack<object *> stack;
                std::set<object *> processed;
            };

            object(object_pool *pool);
            object(object_pool *pool, object *proto);
            object(object_pool *pool, object *proto_0, object *proto_1);
            virtual ~object();
#ifdef MODEL_DEBUG
            int get_id() { return id; }
#else
            int get_id() { return 0; }
#endif
            virtual void kill(object_pool *pool);
            virtual void trace();
            virtual void trace_parallel(object_pool *pool);
            inline void mark();
            void put_off(object_pool *pool);
            void mark_parallel(object_pool *pool);
            inline void unmark(bool flag);
            inline void sweep(object_pool *pool);
            void lock() { immutable = true; }
#ifdef GC_DEBUG
            bool is_died() { return died; }
#endif

            virtual object_type get_type() const;
            virtual object_string *to_object_string();
            virtual object_function *to_object_function();
            virtual object_thread *to_object_thread();
            virtual object_integer *to_object_integer();
            virtual object_real *to_object_real();
            virtual object_void *to_object_void();
            virtual object_undefined *to_object_undefined();
            virtual object_null *to_object_null();
            virtual object_boolean *to_object_boolean();
            virtual object_array_like *to_object_array_like();
            virtual object_array *to_object_array();
            virtual object_exception *to_object_exception();
            virtual object_char *to_object_char();
            virtual object_uid *to_object_uid();
            virtual object_runner *to_object_runner();
            virtual object_string_builder *to_object_string_builder();
            virtual object_byte_array *to_object_byte_array();
            virtual object_file *to_object_file();
            virtual object_port *to_object_port();

            virtual bool less(const object *obj) const;
            virtual std::wstring to_string() const;
            virtual std::wstring to_string_notation() const;
            void copy_objects_to(object *dst);
            void copy_proto_to(object *dst);
            void tsort(tsort_data &data);
            bool instance_of(object *base);
            void flat(object *dst);
            std::vector<object*> get_keys();

            bool add_object(object *key, variable &value);
            bool add_object(object *key, object *value);
            bool add_object(object *key_value) { return add_object(key_value, key_value); }
            variable *find_object(object *key);
            variable *find_own_object(object *key);
            object_function *find_own_method(object_string *key);
            void find_own_and_call_if_exists(thread *thr, int arg_count, object_string *key, call_mode mode);
            bool find_and_vcall(thread *thr, int arg_count, std::wstring name);
            void for_each_proto(std::function<void(object*)> callback);

            virtual bool get_integer(int64_t *pval);
            virtual bool get_real(double *pval);
            virtual bool get_boolean(bool *pval);
            virtual bool get_char(wchar_t *pval);
            virtual bool is_void();

            virtual void op_new(thread *thr, int arg_count);
            virtual void op_add(thread *thr);
            virtual void op_sub(thread *thr);
            virtual void op_pos(thread *thr);
            virtual void op_neg(thread *thr);
            virtual void op_inc(thread *thr);
            virtual void op_dec(thread *thr);
            virtual void op_not(thread *thr);
            virtual void op_bool(thread *thr);
            virtual void op_inv(thread *thr);
            virtual void op_mul(thread *thr);
            virtual void op_exp(thread *thr);
            virtual void op_div(thread *thr);
            virtual void op_mod(thread *thr);
            virtual void op_eq(thread *thr);
            virtual void op_neq(thread *thr);
            virtual void op_shl(thread *thr);
            virtual void op_shr(thread *thr);
            virtual void op_shrz(thread *thr);
            virtual void op_less(thread *thr);
            virtual void op_leq(thread *thr);
            virtual void op_great(thread *thr);
            virtual void op_greq(thread *thr);
            virtual void op_inherit(thread *thr);
            virtual void op_and(thread *thr);
            virtual void op_or(thread *thr);
            virtual void op_bitand(thread *thr);
            virtual void op_bitor(thread *thr);
            virtual void op_xor(thread *thr);
            virtual void op_protect(thread *thr);

            virtual void m_clone(thread *thr, int arg_count);
            virtual void m_instance_of(thread *thr, int arg_count);
            virtual void m_flat(thread *thr, int arg_count);
            virtual void m_get(thread *thr, int arg_count);
            virtual void m_set(thread *thr, int arg_count);
            virtual void m_iterator(thread *thr, int arg_count);
            virtual void m_next(thread *thr, int arg_count);
            virtual void m_valid(thread *thr, int arg_count);

            object *prev;
            object *next;

        protected:
#ifdef MODEL_DEBUG
            int id;
#endif
            int marked;
            bool immutable;
#ifdef GC_DEBUG
            bool died;
#endif
            std::map<object*, variable, object_comparator> objects;
            object *proto;
            lib::pointer<topology_descriptor> topology;
        };

        class generic_object : public object
        {
        friend class object_pool;
        public:
#ifndef GC_DEBUG
            void kill(object_pool *pool) override;
#endif            
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
            static handler *get_instance_ref_var();
            static handler *get_instance_integer();
            static handler *get_instance_real();
            static handler *get_instance_boolean();
            static handler *get_instance_char();

            virtual variable deref(variable *var);

            virtual std::wstring to_string(const variable *var) const = 0;
            virtual std::wstring to_string_notation(const variable *var) const = 0;
            virtual object *to_object(variable *var, object_pool *pool) = 0;
            
            virtual object* get_object(variable *var);
            virtual bool get_integer(variable *var, int64_t *val);
            virtual bool get_real(variable *var, double *val);
            virtual bool get_boolean(variable *var, bool *val);
            virtual bool get_char(variable *var, wchar_t *val);

            virtual void op_add(variable *var, thread *thr);
            virtual void op_sub(variable *var, thread *thr);
            virtual void op_pos(variable *var, thread *thr);
            virtual void op_neg(variable *var, thread *thr);
            virtual void op_inc(variable *var, thread *thr);
            virtual void op_dec(variable *var, thread *thr);
            virtual void op_not(variable *var, thread *thr);
            virtual void op_bool(variable *var, thread *thr);
            virtual void op_inv(variable *var, thread *thr);
            virtual void op_mul(variable *var, thread *thr);
            virtual void op_exp(variable *var, thread *thr);
            virtual void op_div(variable *var, thread *thr);
            virtual void op_mod(variable *var, thread *thr);
            virtual void op_eq(variable *var, thread *thr);
            virtual void op_neq(variable *var, thread *thr);
            virtual void op_shl(variable *var, thread *thr);
            virtual void op_shr(variable *var, thread *thr);
            virtual void op_shrz(variable *var, thread *thr);
            virtual void op_less(variable *var, thread *thr);
            virtual void op_leq(variable *var, thread *thr);
            virtual void op_great(variable *var, thread *thr);
            virtual void op_greq(variable *var, thread *thr);
            virtual void op_inherit(variable *var, thread *thr);
            virtual void op_and(variable *var, thread *thr);
            virtual void op_or(variable *var, thread *thr);
            virtual void op_bitand(variable *var, thread *thr);
            virtual void op_bitor(variable *var, thread *thr);
            virtual void op_xor(variable *var, thread *thr);
            virtual void op_protect(variable *var, thread *thr);

            virtual void m_clone(variable *var, thread *thr, int arg_count);
            virtual void m_instance_of(variable *var, thread *thr, int arg_count);
            virtual void m_flat(variable *var, thread *thr, int arg_count);
            virtual void m_iterator(variable *var, thread *thr, int arg_count);
            virtual void m_next(variable *var, thread *thr, int arg_count);
            virtual void m_valid(variable *var, thread *thr, int arg_count);
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

        void variable::set_reference(variable *ref)
        {
            hndl = handler::get_instance_ref_var();
            data.ref = ref;
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

        void variable::set_char(wchar_t value)
        {
            hndl = handler::get_instance_char();
            data.c = value;
        }

        variable variable::deref()
        {
            return hndl->deref(this);
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

        void variable::mark()
        {
            object *obj = hndl->get_object(this);
            if (obj)
            {
                obj->mark();
            }
        }

        void variable::mark_parallel(object_pool *pool)
        {
            object *obj = hndl->get_object(this);
            if (obj)
            {
                obj->mark_parallel(pool);
            }
        }

        void variable::put_off(object_pool *pool)
        {
            object *obj = hndl->get_object(this);
            if (obj)
            {
                obj->put_off(pool);
            }
        }

        object *variable::get_object()
        {
            return hndl->get_object(this);
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

        bool variable::get_char(wchar_t *pval)
        {
            return hndl->get_char(this, pval);
        }

        bool variable::get_byte(uint8_t *pval)
        {
            int64_t val;
            if (get_integer(&val))
            {
                if (val >= 0 && val <= 255)
                {
                    *pval = (uint8_t)val;
                    return true;
                }
            }
            return false;
        }

        void variable::op_add(thread *thr)
        {
            hndl->op_add(this, thr);
        }

        void variable::op_sub(thread *thr)
        {
            hndl->op_sub(this, thr);
        }

        void variable::op_pos(thread *thr)
        {
            hndl->op_pos(this, thr);
        }

        void variable::op_neg(thread *thr)
        {
            hndl->op_neg(this, thr);
        }

        void variable::op_inc(thread *thr)
        {
            hndl->op_inc(this, thr);
        }

        void variable::op_dec(thread *thr)
        {
            hndl->op_dec(this, thr);
        }

        void variable::op_not(thread *thr)
        {
            hndl->op_not(this, thr);
        }

        void variable::op_bool(thread *thr)
        {
            hndl->op_bool(this, thr);
        }

        void variable::op_inv(thread *thr)
        {
            hndl->op_inv(this, thr);
        }

        void variable::op_mul(thread *thr)
        {
            hndl->op_mul(this, thr);
        }

        void variable::op_exp(thread *thr)
        {
            hndl->op_exp(this, thr);
        }

        void variable::op_div(thread *thr)
        {
            hndl->op_div(this, thr);
        }

        void variable::op_mod(thread *thr)
        {
            hndl->op_mod(this, thr);
        }

        void variable::op_eq(thread *thr)
        {
            hndl->op_eq(this, thr);
        }

        void variable::op_shl(thread *thr)
        {
            hndl->op_shl(this, thr);
        }

        void variable::op_shr(thread *thr)
        {
            hndl->op_shr(this, thr);
        }

        void variable::op_shrz(thread *thr)
        {
            hndl->op_shrz(this, thr);
        }

        void variable::op_less(thread *thr)
        {
            hndl->op_less(this, thr);
        }

        void variable::op_leq(thread *thr)
        {
            hndl->op_leq(this, thr);
        }

        void variable::op_great(thread *thr)
        {
            hndl->op_great(this, thr);
        }

        void variable::op_greq(thread *thr)
        {
            hndl->op_greq(this, thr);
        }

        void variable::op_neq(thread *thr)
        {
            hndl->op_neq(this, thr);
        }

        void variable::op_inherit(thread *thr)
        {
            hndl->op_inherit(this, thr);
        }

        void variable::op_and(thread *thr)
        {
            hndl->op_and(this, thr);
        }

        void variable::op_or(thread *thr)
        {
            hndl->op_or(this, thr);
        }

        void variable::op_bitand(thread *thr)
        {
            hndl->op_bitand(this, thr);
        }

        void variable::op_bitor(thread *thr)
        {
            hndl->op_bitor(this, thr);
        }

        void variable::op_xor(thread *thr)
        {
            hndl->op_xor(this, thr);
        }
        
        void variable::op_protect(thread *thr)
        {
            hndl->op_protect(this, thr);
        }

        void variable::m_clone(thread *thr, int arg_count)
        {
            hndl->m_clone(this, thr, arg_count);
        }

        void variable::m_instance_of(thread *thr, int arg_count)
        {
            hndl->m_instance_of(this, thr, arg_count);
        }
        
        void variable::m_flat(thread *thr, int arg_count)
        {
            hndl->m_flat(this, thr, arg_count);
        }

        void variable::m_iterator(thread *thr, int arg_count)
        {
            hndl->m_iterator(this, thr, arg_count);
        }

        void variable::m_next(thread *thr, int arg_count)
        {
            hndl->m_next(this, thr, arg_count);
        }

        void variable::m_valid(thread *thr, int arg_count)
        {
            hndl->m_valid(this, thr, arg_count);
        }

        /*
            Object inline methods
        */
        void object::mark()
        {
            if (marked < 1)
            {
                marked = 1;

                for (auto pair : objects)
                {
                    pair.first->mark();
                    pair.second.mark();
                }

                if (proto)
                {
                    proto->mark();
                }
                else if (topology)
                {
                    for (int i = 0, size = topology->proto.size(); i < size; i++)
                        topology->proto[i]->mark();
                }
                

                trace();
            }
        }

        void object::unmark(bool flag)
        {
            marked = flag ? -1 : 0;
        }

        void object::sweep(object_pool *pool)
        {
            if (marked == 0)
            {
#if 0
                std::wcout << L"killed: " << id << L", " << to_string() << std::endl;
#endif
                kill(pool);
            }
            else
            {
                unmark(false);
            }
        }
    };
};
