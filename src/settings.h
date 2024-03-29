/*

Copyright (C) 2017-2021 Ivan Kniazkov

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
#define STACK_DEBUG
//#define GC_DEBUG
#define GC_DEFAULT_PARALLEL

#define GPIO_ENABLE
#define GPIO_EMULATION
//#define GPIO_DEBUG

/**
 * Allows you to describe functions without argument block or (and)
 * without body, for example:
 * 
 * var f = $ { return 2 + 3; }; // without arguments
 * var f = $ (); // without a body
 * var f = $; // empty function (does nothing)
 */
#define FEATURE_SHORT_FUNC_DECLARATION
