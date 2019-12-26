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

#include "vm.h"
#include "vm_exception.h"
#include "debug_mode_info.h"
#include "model/object.h"
#include "model/object_string.h"
#include "code/disasm.h"
#include "lib/assert.h"
#include "lib/utils.h"
#include <iostream>
#include <sstream>
#include <climits>

namespace g0at
{
    namespace vm
    {
        vm::vm(lib::pointer<code::code> _code)
            : code(_code)
        {
        }

        int vm::run(environment *env)
        {
            model::variable ret;
            model::process *proc = env->get_process();
            model::thread *thr = proc->active_threads->create_thread(env->get_context(), &ret);
            ret.set_object(env->get_pool()->get_undefined_instance());
            thr->iid = code::iid_t(0);
            thr->next = thr;
            thr->state = model::thread_state::ok;
            bool stop = false;
            try
            {
                if (!env->debug_mode())
                {
                    while(!stop)
                    {
                        model::thread *thrd = proc->active_threads->process_delayed_threads();
                        if (thr == nullptr) thr = thrd;
                        if (thr != nullptr)
                        {
                            code::iid_t iid = thr->iid;
                            ++thr->iid;
                            auto instr = code->get_instruction(iid);
                            instr->exec(thr);
                            env->get_gc()->collect_garbage_if_necessary();
                            thr = proc->active_threads->switch_thread(&stop);
                        }
                    }
                }
                else
                {
                    debug_mode_info debug_info;
                    if (!env->run_mode())
                    {
                        while(true)
                        {
                            std::cout << "? ";
                            std::string line = lib::get_line();
                            if (line == "r")
                            {
                                break;
                            }
                            else if (line.find("b ") == 0)
                            {
                                std::string request = lib::trim(line.substr(2));
                                lib::pointer<breakpoint> bp = env->get_listing()->set_breakpoint(request);
                                if (bp)
                                {
                                    std::cout << global::char_encoder->encode(global::resource->setting_breakpoint_at(bp->to_string())) << std::endl;
                                    debug_info.breakpoints.push_back(bp);
                                }
                                else
                                {
                                    std::cout << global::char_encoder->encode(global::resource->can_not_set_breakpoint()) << std::endl;
                                }
                            }
                            else if (line != "")
                            {
                                std::cout << global::char_encoder->encode(global::resource->syntax_error()) << std::endl;
                            }
                        }
                    }
                    long int ticks = 0;
                    while(!stop)
                    {
                        model::thread *thrd = proc->active_threads->process_delayed_threads();
                        if (thr == nullptr) thr = thrd;
                        if (thr == nullptr)
                            continue;
                        code::iid_t iid = thr->iid;
                        ++thr->iid;
                        auto instr = code->get_instruction(iid);
                        bool suspend = instr->exec_debug(thr, &debug_info);
                        if (suspend)
                        {
                            source_manager *listing = env->get_listing();
                            lib::pointer<position> pos = listing->get_position_by_absolute_position(debug_info.frame_begin);
                            std::wstring frag = listing->get_fragment(debug_info.frame_begin, debug_info.frame_end);
                            std::cout << '\n' << ticks << " -> " << global::char_encoder->encode(pos->to_string())
                                    << '\n' << global::char_encoder->encode(frag) << "\n";

                            while (true)
                            {
                                std::cout << "? ";

                                switch (thr->debug_state)
                                {
                                    case model::thread_debug_state::do_not_stop:
                                        std::cout << "[c] ";    // continue
                                        break;
                                    case model::thread_debug_state::step_into:
                                        std::cout << "[e] ";    // enter
                                        break;
                                    case model::thread_debug_state::step_over:
                                        std::cout << "[n] ";    // next
                                        break;
                                    case model::thread_debug_state::step_out:
                                        std::cout << "[l] ";    // leave
                                        break;
                                    default:
                                        break;
                                }

                                std::string line = lib::get_line();
                                if (line == "")                 // default value
                                {
                                    break;
                                }
                                else if (line == "c")           // continue
                                {
                                    thr->debug_state = model::thread_debug_state::do_not_stop;
                                    break;
                                }
                                else if (line == "e")           // enter
                                {
                                    thr->debug_state = model::thread_debug_state::step_into;
                                    break;
                                }
                                else if (line == "n")           // next
                                {
                                    thr->debug_state = model::thread_debug_state::step_over;
                                    break;
                                }
                                else if (line == "l")           // leave
                                {
                                    thr->debug_state = model::thread_debug_state::step_out;
                                    break;
                                }
                                else if (line.find("b ") == 0)
                                {
                                    std::string request = lib::trim(line.substr(2));
                                    lib::pointer<breakpoint> bp = env->get_listing()->set_breakpoint(request);
                                    if (bp)
                                    {
                                        std::cout << global::char_encoder->encode(global::resource->setting_breakpoint_at(bp->to_string())) << std::endl;
                                        debug_info.breakpoints.push_back(bp);
                                    }
                                    else
                                    {
                                        std::cout << global::char_encoder->encode(global::resource->can_not_set_breakpoint()) << std::endl;
                                    }
                                }
                                else if (line.find("p ") == 0)
                                {
                                    std::wstring request = global::char_encoder->decode(lib::trim(line.substr(2)));
                                    std::vector<std::wstring> list;
                                    size_t begin = 0, end;
                                    while(std::string::npos != (end = request.find(L'.', begin)))
                                    {
                                        list.push_back(request.substr(begin, end - begin));
                                        begin = end + 1;
                                    }
                                    list.push_back(request.substr(begin));
                                    model::object *obj = thr->ctx;
                                    size_t i = 0, l = list.size();
                                    if (list[0] == L"this")
                                    {
                                        obj = thr->ctx->this_ptr;
                                        if (!obj)
                                            obj = thr->pool->get_undefined_instance();
                                        i++;
                                    }
                                    for (; i < l; i++)
                                    {
                                        model::object_string *key = thr->pool->get_static_string(list[i]);
                                        model::variable *ref = obj->find_object(key);
                                        if(ref != nullptr)
                                        {
                                            obj = ref->to_object(thr->pool);
                                        }
                                        else
                                        {
                                            if (i == l - 1)
                                                obj = thr->pool->get_undefined_instance();
                                            else
                                            {
                                                obj = nullptr;
                                                std::wstringstream partial_name;
                                                for (int j = 0; j <= i; j++)
                                                {
                                                    if (j)
                                                        partial_name << L".";
                                                    partial_name << list[j];
                                                }
                                                std::cout
                                                    << global::char_encoder->encode(global::resource->illegal_reference_variable_is_not_defined(partial_name.str()))
                                                    << std::endl;
                                                break;
                                            }
                                        }
                                    }
                                    if (obj != nullptr)
                                    {
                                        std::cout << global::char_encoder->encode(obj->to_string_notation()) << std::endl;
                                    }
                                }
                            }

                            switch (thr->debug_state)
                            {
                                case model::thread_debug_state::step_into:
                                    thr->debug_level++;
                                    break;
                                case model::thread_debug_state::step_out:
                                    thr->debug_level--;
                                    break;
                                default:
                                    break;
                            }
                        }
                        if (!thr->stack_is_empty())
                        {
                            // convert any value to real object
                            thr->peek().to_object(env->get_pool());
                        }
                        env->get_gc()->collect_garbage_if_necessary();
                        thr = proc->active_threads->switch_thread(&stop);
                        ticks++;
                    }
                }

                int ret_value = 0;
                int64_t ret_value_int64;
                if (ret.get_integer(&ret_value_int64) && ret_value_int64 >= INT_MIN && ret_value_int64 <= INT_MAX)
                    ret_value = (int)ret_value_int64;

                if (env->debug_mode() && !env->run_mode())
                    std::cout << '\n' << global::char_encoder->encode(global::resource->program_terminated_with_exit_code(ret_value)) << '\n';

                return ret_value;
            }
            catch(const vm_exception &vmex)
            {
                std::cerr << global::char_encoder->encode(code::disasm::to_string(code, thr->iid - 10, thr->iid)); 
                throw;
            }
        }
    };
};
