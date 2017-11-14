#include "Unicode.h"
#include "WideStringBuilder.h"

namespace goat {

	RawString Unicode::BadUTF8::toRawString() {
		return L"bad UTF8";
	}

	WideString Unicode::UTF8Decode(String s) {
		unsigned int i = 0,
			len = s.len();
		WideStringBuilder result;
		while (i < len) {
			unsigned char c0 = (unsigned char)s[i];
			wchar w = 0;
			if ((c0 & 0x80) == 0) {
				w = c0;
				i++;
			}
			else if ((c0 & 0xE0) == 0xC0) {
				if (i + 1 >= len) {
					throw BadUTF8();
				}
				unsigned char c1 = (unsigned char)s[i+1];
				if ((c1 & 0xC0) != 0x80) {
					throw BadUTF8();
				}
				w = ((c0 & 0x1F) << 6) + (c1 & 0x3F);
				i += 2;
			}
			else {
				throw BadUTF8();
			}
			result << w;
		}
		return result.toWideString();
	}

	unsigned int Unicode::UTF8EncodeChar(wchar w, char *c) {
		if (w < 0x80) {
			c[0] = (char)w;
			return 1;
		}
		if (w < 0x800) {
			c[0] = (char)((w & 0x7C0) >> 6) + 0xC0;
			c[1] = (char)(w & 0x3F) + 0x80;
			return 2;
		}
		return 0;
	}

}