@echo off
if exist tests\%1 goto end
echo create tests\%1...
mkdir tests\%1
echo. > tests\%1\program.goat
"C:\Program Files\Notepad++\notepad++.exe" tests\%1\program.goat
:end