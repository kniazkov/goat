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
#include "debug_mode_info.h"
#include "model/object.h"
#include "lib/assert.h"
#include "lib/utils.h"
#include <iostream>
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
            model::thread *thr = env->get_thread_list()->create_thread(env->get_context(), &ret);
            ret.set_object(env->get_pool()->get_undefined_instance());
            thr->iid = code::iid_t(0);
            thr->next = thr;
            thr->state = model::thread_state::ok;
            if (!env->debug_mode())
            {
                while(thr != nullptr)
                {
                    code::iid_t iid = thr->iid;
                    ++thr->iid;
                    auto instr = code->get_instruction(iid);
                    instr->exec(thr);
                    env->get_gc()->collect_garbage_if_necessary();
                    thr = env->get_thread_list()->switch_thread();
                }
            }
            else
            {
                debug_mode_info debug_info;
                while(thr != nullptr)
                {
                    code::iid_t iid = thr->iid;
                    ++thr->iid;
                    auto instr = code->get_instruction(iid);
                    bool suspend = instr->exec_debug(thr, &debug_info);
                    if (suspend)
                    {
                        source_manager *listing = env->get_listing();
                        lib::pointer<position> pos = listing->get_position_by_absolute_position(debug_info.frame_begin);
                        std::wstring frag = listing->get_fragment(debug_info.frame_begin, debug_info.frame_end);
                        std::cout << '\n' << global::char_encoder->encode(pos->to_string()) 
                                  << '\n' << global::char_encoder->encode(frag) << '\n';
                        switch (debug_info.state)
                        {
                            case debug_mode_state::step_over:
                                std::cout << "n "; // next
                                break;
                            case debug_mode_state::step_into:
                                std::cout << "e "; // enter
                                break;
                            case debug_mode_state::step_out:
                                std::cout << "l "; // leave
                                break;
                        }
                        std::cout << "? ";
                        std::string line;
                        std::getline(std::cin, line);
                        line = lib::trim(line);
                        
                        if (line == "c") // continue
                            debug_info.state = debug_mode_state::do_not_stop;
                        else if (line == "n") // next
                            debug_info.state = debug_mode_state::step_over;
                        else if (line == "e") // enter
                            debug_info.state = debug_mode_state::step_into;
                        else if (line == "l") // leave
                            debug_info.state = debug_mode_state::step_out;

                        switch (debug_info.state)
                        {
                            case debug_mode_state::step_into:
                                debug_info.set_level++;
                                break;
                            case debug_mode_state::step_out:
                                debug_info.set_level--;
                                break;
                        }
                    }
                    if (!thr->stack_is_empty())
                    {
                        // convert any value to real object
                        thr->peek().to_object(env->get_pool());
                    }
                    env->get_gc()->collect_garbage_if_necessary();
                    thr = env->get_thread_list()->switch_thread();
                }
            }

            int64_t ret_value_int64;
            if (ret.get_integer(&ret_value_int64) && ret_value_int64 >= INT_MIN && ret_value_int64 <= INT_MAX)
                return (int)ret_value_int64;
            return 0;
        }
    };
};
