@echo off
if "%1"=="" goto NO_PARAM
if not exist goat.exe goto NO_GOAT
if exist test\%1 goto ALREADY_EXIST
echo Create test '%1'...
mkdir test\%1
copy program.goat test\%1\
python test.py goat.exe -select=%1
if exist test\%1\output_act.txt rename test\%1\output_act.txt output.txt
if exist test\%1\error_act.txt rename test\%1\error_act.txt error.txt
python test.py goat.exe -select=%1
git add test\%1\*
git commit -am "+ test"
echo Done.
goto END

:NO_PARAM
  echo Usage:
  echo   create_test.cmd test_name
  goto END
  
:ALREADY_EXIST
  echo The test '%1' already exists!
  goto END

:NO_GOAT
  echo goat.exe not found
  goto END

:END
