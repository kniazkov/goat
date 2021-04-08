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

#include "dll_loader.h"
#include "utils.h"

#ifdef _WIN32
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

namespace g0at
{
    namespace lib
    {
        dll_loader * dll_loader::open(std::string file_name)
        {
            std::string path = normalize_file_path(file_name);
            void *dll = nullptr;
#ifdef _WIN32
            dll = (void*)LoadLibraryA(path.c_str());
#else
            dll = dlopen(path.c_str(), RTLD_NOW);
#endif   
            if (dll == nullptr)
                return nullptr;

            return new dll_loader(dll);         
        }

        dll_loader::~dll_loader()
        {
#ifdef _WIN32
            FreeLibrary((HMODULE)dll_instance);
#else
            dlclose(dll_instance);
#endif
        }

        void * dll_loader::load(std::string function_name)
        {
#ifdef _WIN32
            return (void*)GetProcAddress((HMODULE)dll_instance, function_name.c_str());
#else
            return (void*)dlsym(dll, function_name.c_str());
#endif
        }
    }
}
