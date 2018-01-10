# Copyright (C) 2017, 2018 Ivan Kniazkov

# This file is part of interpreter of programming language
# codenamed "Goat" ("Goat interpreter").

# Goat interpreter is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Goat interpreter is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along
# with Goat interpreter.  If not, see <http://www.gnu.org/licenses/>.

import sys
import os
import os.path
import subprocess
import time

if len(sys.argv) < 2:
    print("usage: python test.py <path_to_interpreter>");
    exit()

print("testing...")

interpreter = sys.argv[1]
all = False
showFail = False
gcDebug = False
select = False
if len(sys.argv) > 2 :
	for i in range(2, len(sys.argv)) :
		if sys.argv[i] == "-all" :
			all = True
		if sys.argv[i] == "-failed" :
			showFail = True
		if sys.argv[i] == "-gcdebug" :
			gcDebug = True
		if sys.argv[i].startswith("-select=") :
			select = sys.argv[i][8:]
			
if os.path.isfile(interpreter) != True :
    print("file not exists: " + interpreter);
    exit()

d = 'tests'
passed = 0
failed = 0
for dir in ([os.path.join(d, o) for o in os.listdir(d) if ((select == False and (o[0] != '_' or all)) or (select != False and select in o)) and os.path.isdir(os.path.join(d, o))]):
	prog = os.path.join(dir, "program.goat")
	if os.path.isfile(prog) :
		out = False
		err = False
		out_0 = os.path.join(dir, "output.txt")
		if os.path.isfile(out_0) :
			out_0 = open(out_0, "rb").read()
			out = True
		else :
			out_0 = "nothing"
		err_0 = os.path.join(dir, "error.txt")
		if os.path.isfile(err_0) :
			err_0 = open(err_0, "rb").read()
			err = True
		else :
			err_0 = "nothing"
		equal = True
		begin = time.time()
		if gcDebug :
			proc = subprocess.Popen([interpreter, prog, "--gc=debug", "--lib=../lib"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		else :
			proc = subprocess.Popen([interpreter, prog, "--lib=../lib"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		out_1, err_1 = proc.communicate()
		ret = proc.wait()
		end = time.time()
		strTime = ("%.3f" % (end - begin))
		if (out and (out_0 != out_1)) or (out != True and len(out_1) > 0):
			print("out 0: " + str(out_0))
			print("out 1: " + str(out_1))
			actual = open(os.path.join(dir, "output_act.txt"), "wb")
			actual.write(out_1)
			actual.close()
			equal = False
		if (err and (err_0 != err_1)) or (err != True and len(err_1) > 0):
			print("err 0: " + str(err_0))
			print("err 1: " + str(err_1))
			actual = open(os.path.join(dir, "error_act.txt"), "wb")
			actual.write(err_1)
			actual.close()
			equal = False
		if equal :
			if not showFail :
				print("[" + " ok " + "] " + strTime + " " + dir)
			passed = passed + 1
		else :
			print("[" + "fail" + "] " + strTime + " " + dir)
			failed = failed + 1
  
print("done.")
print("total " + str(passed + failed) + ", passed " + str(passed) + ", failed " + str(failed))
