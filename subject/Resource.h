/*

Copyright (C) 2017, 2018 Ivan Kniazkov

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

#include "Type.h"
#include "String.h"
#include "WideString.h"

namespace goat {

	class Resource {
	public:
		static int32 i_arguments();
		static int32 i_init();
		static int32 i_trace();
		static int32 i_clone();

		static String s_arguments;
		static String s_init;
		static String s_trace;
		static String s_clone;
		static String s_true;
		static String s_false;
		static String s_undefined;
		static String s_null;
		static String s_breakStmt;
		static String s_continueStmt;
		static String s_var;
		static String s_function;
		static String s_return;
		static String s_if;
		static String s_else;
		static String s_for;
		static String s_do;
		static String s_while;
		static String s_new;
		static String s_thread;
		static String s_break;
		static String s_continue;
		static String s_in;
		static String s_switch;
		static String s_case;
		static String s_default;
		static String s_try;
		static String s_catch;
		static String s_finally;
		static String s_throw;
		static String s_import;
		static String s_this;
		static String s_debug;
		static String s_debugStmt;
		static String s_void;

		static String s_colon;
		static String s_semicolon;
		static String s_comma;
		static String s_dot;
		static String s_nullGuard;

		static WideString w_void;
		static WideString w_undefined;
		static WideString w_null;
		static WideString w_File;

		static WideString w_continue;
		static WideString w_step;
		static WideString w_into;
		static WideString w_out;
	};

}