if exist goat.exe del goat.exe
if not exist build mkdir build
cd build
cmake ..\src -G "MinGW Makefiles"
mingw32-make
cd ..
copy build\goat.exe .
