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

#include "context.h"
#include "object_cache.h"
#include "stack.h"
#include "code/iid_t.h"
#include <map>
#include <vector>

namespace g0at
{
    namespace model
    {
        class process;
        class thread_list;

        /**
         * @brief State of thread
         */
        enum class thread_state
        {
            // Thread is paused
            pause,
            // Thread is runned
            ok,
            // Thread is finished and should be deleted
            zombie
        };

        /**
         * @brief Additional state of thread in debug mode
         */
        enum class thread_debug_state
        {
            // Resumes code execution until the next breakpoint is hit
            do_not_stop,
            // Runs the next statement. If the current line contains a function call, steps into the most deeply nested function
            step_into,
            // Runs the next statement without stepping into functions or methods
            step_over,
            // Continues running code and suspends execution when the current function returns
            step_out
        };

        /**
         * @brief Order of execution of branching instructions
         */
        enum class thread_flow
        {
            // Execute all blocks
            direct,
            // Execute all 'finally' blocks until end of function is reached
            descent_return,
            // Execute all 'finally' blocks until 'catch' block is found
            descent_exception,
            // Execute all 'finally' blocks until end of cycle is found
            descent_break,
            // Execute all 'finally' blocks until end of cycle is found, then go to begin of cycle
            descent_continue
        };

        /**
         * @brief Thread identifier
         * 
         * Each thread has a unique identifier and threads can be found by this.
         */
        class thread_id
        {
        public:
            thread_id()
                : value(-1)
            {
            }

            thread_id(int64_t _value)
                : value(_value)
            {
            }

            bool operator<(const thread_id &tid) const
            {
                return value < tid.value;
            }

            int64_t as_int64() { return value; }
            bool valid() { return value >= 0; }
        
        private:
            int64_t value;
        };

        /**
         * @brief Thread description
         */
        class thread
        {
            friend class thread_list;
            friend class thread_list_ext;
        protected:
            thread(process *_proc, thread_id _tid, context *_ctx, object_pool *_pool, variable *_ret);

        public:
            /**
             * @brief Raises an exception
             * 
             * Restores stack until 'catch' or 'finally' block is found,
             * then sets instruction index to the begin of this block.
             * 
             * @param var A variable that will be passed to a 'catch' block.
             */
            void raise_exception(variable &var);
            void mark_all();
            void mark_all_parallel();
            void resume();
            void kill();

            process *get_process() { return proc; }
            thread_id get_id() { return tid; }
            variable *push(variable var) { return data.push(var); }
            variable pop() { return data.pop(); }
            void pop(int n) { data.pop(n); }
            variable &peek() { return data.peek(); }
            variable &peek(int n) { return data.peek(n); }
            bool stack_is_empty() { return data.empty(); }

            variable *push_undefined()
            {
                variable var;
                var.set_object(pool->get_undefined_instance());
                return data.push(var);
            }

            /**
             * @brief Raises an exception (wrapper)
             * 
             * @param obj An object that will be passed to a 'catch' block.
             */
            void raise_exception(object *obj)
            {
                variable var;
                var.set_object(obj);
                raise_exception(var);
            }

            void set_context(context *_ctx)
            {
                ctx = _ctx;
                ctx->stack_size = data.size();
                ctx->lock = lock;
            }

            void restore_context()
            {
                if (ctx)
                {
                    data.restore_size(ctx->stack_size);
                    lock = ctx->lock;
                    ctx->ret = nullptr;
                    ctx = ctx->prev;
                }
                else
                {
                    data.restore_size(0);
                    lock = 0;
                }
            }

            thread *prev;
            thread *next;
            bool is_active;
            code::iid_t iid;
            thread_state state;
            thread_flow flow;
            context *ctx;
            object_pool *pool;
            variable *ret;
            variable except;
            std::vector<thread_id> joined;
            object *runner;
            int lock;
            int debug_level;
            thread_debug_state debug_state;
            lib::pointer<position> line;

        protected:
            thread(const thread &) { }
            void operator=(const thread &) { }

            stack data;
            process *proc;
            thread_id tid;
        };

        class thread_list
        {
            friend class thread_list_ext;
        public:
            thread_list(process *_proc);
            virtual ~thread_list();
            void add_thread(thread* new_thr);
            void remove_current_thread();
            void remove_thread(thread *thr);

            process *get_process() { return proc; }
            thread *get_current_thread() { return current; }
            
        protected:
            thread_list(const thread_list &) { }
            void operator=(const thread_list &) { }

            process *proc;
            thread *current;
        };

        class thread_list_ext : public thread_list
        {
        public:
            thread_list_ext(process *_proc, thread_list *_aux_list);
            thread *create_thread(context *_ctx, variable *_ret, object_pool *_pool);
            thread *create_delayed_thread(context *_ctx, int64_t _delay, object_pool *_pool);
            thread *switch_thread(bool *stop);
            thread *process_delayed_threads();

            thread *get_thread_by_tid(thread_id tid)
            {
                auto iter = thread_by_tid.find(tid);
                return iter != thread_by_tid.end() ? iter->second : nullptr;
            }

        protected:
            thread_list_ext(const thread_list_ext &) : thread_list(nullptr) { }
            void operator=(const thread_list_ext &) { }

            thread_list *aux_list;
            std::map<thread_id, thread*> thread_by_tid;
            std::map<int64_t, thread_id> delayed_threads;
            thread_id last_tid;
        };
    };
};