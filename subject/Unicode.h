#pragma once

#include "String.h"
#include "WideString.h"
#include "Exception.h"

namespace goat {

	class Unicode {
	public:
		class BadUTF8 : public Exception {
		public:
			RawString toRawString() override;
		};

		static WideString UTF8Decode(String s);
		static unsigned int UTF8EncodeChar(wchar w, char *c);
	};

}