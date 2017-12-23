if exist tests\%1 goto EOF
mkdir tests\%1
echo. > tests\%1\program.goat
"C:\Program Files\Notepad++\notepad++.exe" tests\%1\program.goat
