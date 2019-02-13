#pragma once
#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstdarg>

namespace levdb {
	namespace utils {
		const int kPrintBuffer = 1 << 12;

		void HandleCheckError(const char *msg) {
			fprintf(stderr, "%s\n", msg);
			exit(-1);
		}

		void HandleAssertError(const char *msg) {
			fprintf(stderr, "AssertError:%s\n", msg);
			exit(-1);
		}

		void Check(bool exp, const char *fmt, ...) {
			if (!exp) {
				std::string msg(kPrintBuffer, '\0');
				va_list args;
				va_start(args, fmt);
				vsnprintf(&msg[0], kPrintBuffer, fmt, args);
				va_end(args);
				HandleCheckError(msg.c_str());
			}
		}

		void Assert(bool exp, const char *fmt, ...) {
			if (!exp) {
				std::string msg(kPrintBuffer, '\0');
				va_list args;
				va_start(args, fmt);
				vsnprintf(&msg[0], kPrintBuffer, fmt, args);
				va_end(args);
				HandleAssertError(msg.c_str());
			}
		}

		void Error(const char *fmt, ...) {
			std::string msg(kPrintBuffer, '\0');
			va_list args;
			va_start(args, fmt);
			vsnprintf(&msg[0], kPrintBuffer, fmt, args);
			va_end(args);
			HandleCheckError(msg.c_str());
		}
	}
}