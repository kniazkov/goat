/*

Copyright (C) 2017 Ivan Kniazkov

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

#include "GarbageCollector.h"
#include "Object.h"
#include "Thread.h"
#include "Memory.h"

namespace goat {

	class GarbageCollectorDebug : public GarbageCollector {
	public:
		void collect() override {
			ObjectList::global.unmark();
			ThreadList::global.mark();
			while (!ObjectList::forMarking.mark_2()) {
			}
			ObjectList::global.sweep();
		}
	};

	class GarbageCollectorSerial : public GarbageCollector {
	protected:
		static const long long threshold = 16 * 1024;
		long long prevAlloc;
		long long prevObjMem;
	public:
		GarbageCollectorSerial() {
			prevAlloc = 0;
			prevObjMem = 0;
		}

		void collect() override {
			if (totalAlloc - prevAlloc > threshold || totalObjMem - prevObjMem > threshold) {
				ThreadList::global.mark();
				while (!ObjectList::forMarking.mark_2()) {
				}
				ObjectList::global.sweep();
				prevAlloc = totalAlloc;
				prevObjMem = totalObjMem;
			}
		}
	};

	class GarbageCollectorParallel : public GarbageCollector {
	protected:
		enum CollectionStage {
			MARK_ROOT,
			MARK,
			SWEEP
		};
		
		CollectionStage stage;
		Object *obj,
			*nextObj;
	public:
		GarbageCollectorParallel() {
			stage = MARK_ROOT;
			obj = nullptr;
			nextObj = nullptr;
		}

		void collect() override {
			int i;

			switch (stage) {
			case MARK_ROOT:
				ThreadList::global.mark();
				stage = MARK;
				break;
			case MARK:
				for (i = 0; i < 2; i++) {
					if (ObjectList::forMarking.mark_2()) {
						stage = SWEEP;
						obj = ObjectList::global.first;
						break;
					}
				}
				break;
			case SWEEP:
				for (i = 0; i < 2; i++) {
					if (obj) {
						nextObj = obj->next;
						if (obj->status == Object::UNMARKED) {
							delete obj;
						}
						else if (obj->status == Object::MARKED) {
							obj->status = Object::UNMARKED;
						}
						obj = nextObj;
					}
					else {
						stage = MARK_ROOT;
					}
				}
				break;
			}
		}
	};

	GarbageCollector * GarbageCollector::debug() {
		static GarbageCollectorDebug collector;
		return &collector;
	}

	GarbageCollector * GarbageCollector::serial() {
		static GarbageCollectorSerial collector;
		return &collector;
	}

	GarbageCollector * GarbageCollector::parallel() {
		static GarbageCollectorParallel collector;
		return &collector;
	}

}