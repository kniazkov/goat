@echo off
if not exist tests\%1\program.goat goto end
if exist tests\%1\error_act.txt del tests\%1\error_act.txt
copy tests\%1\output_act.txt tests\%1\output.txt
del tests\%1\output_act.txt
git add tests\%1
echo validated tests\%1

:end