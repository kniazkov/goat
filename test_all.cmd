@echo off
del /F /Q /S test\*_act.txt
python test.py goat.exe -failed
