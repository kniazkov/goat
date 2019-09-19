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

#include "frame.h"

namespace g0at
{
    namespace code
    {
        _frame::_frame(int _begin, int _end)
            : begin(_begin), end(_end)
        {
        }

        void _frame::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _frame::exec(model::thread *thr)
        {
            // do nothing
        }

        bool _frame::exec_debug(model::thread *thr, vm::debug_mode_info *debug_info)
        {
            debug_info->frame_begin = begin;
            debug_info->frame_end = end;
            if (thr->debug_level > thr->ctx->debug_level)
                thr->debug_level = thr->ctx->debug_level;

            if (thr->debug_state == model::thread_debug_state::do_not_stop)
            {
                for (auto bp : debug_info->breakpoints)
                {
                    if (bp->triggered(begin))
                    {
                        return true;                        
                    }
                }
                return false;
            }
            return thr->debug_level == thr->ctx->debug_level;
        }
    };
};
