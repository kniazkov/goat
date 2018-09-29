/*

Copyright (C) 2017, 2018 Ivan Kniazkov

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

#include "Console.h"
#include "WideStringBuilder.h"

namespace goat {

	WideString Console::read() {
		WideStringBuilder result;
		auto data = input->read();

		while (data.hasValue) {
			if (data.value == terminator || !data.value) {
				break;
			}
			else {
				bool flag = true;
				if (ignore) {
					const wchar *i = ignore;
					while (*i) {
						if (data.value == *i) {
							flag = false;
							break;
						}
						i++;
					}
				}
				if (flag) {
					result << data.value;
				}
			}
			data = input->read();
		}

		return result.toWideString();
	}

	void Console::write(wchar c) {
		output->write(c);
	}

	void Console::write(const wchar *s) {
		output->write(s);
	}

	void Console::write(WideString ws) {
		output->write(ws.cwstr());
	}
}
