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

#include <string>

namespace g0at
{
    namespace lib
    {
        /**
         * @brief The cross-platform loader for dynamic linked libraries
         */
        class dll_loader
        {
        private:
            /**
             * @brief The instance of loaded DLL (depends on the platform)
             */
            void *dll_instance;

            /**
             * @brief The constructor
             */
            dll_loader(void *_dll_instance) : dll_instance(_dll_instance)
            {
            }

        public:
            /**
             * @brief Opens the DLL file
             * @param file_name The DLL file name
             * @return The loader instance or null pointer if failed
             */
            static dll_loader * open(std::string file_name);
            
            /**
             * @brief The destructor
             */
            virtual ~dll_loader();

            /**
             * @brief Loads the function from DLL file
             * @param function_name The nane of the function
             * @return Pointer to the function or null pointer if the function was not found
             */
            void * load(std::string function_name);
        };
    }
}