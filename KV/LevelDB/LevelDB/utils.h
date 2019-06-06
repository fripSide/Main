#pragma once
#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstdarg>
#include <cassert>
#include <thread>

namespace levdb {
	namespace utils {
		void HandleCheckError(const char* msg);

		void HandleAssertError(const char* msg);

		void Check(bool exp, const char* fmt, ...);

		void Assert(bool exp, const char* fmt, ...);

		void Error(const char* fmt, ...);

		void Log(const char* fmt, ...);

		size_t GetThreadId(std::thread::id tid);
	}

	class noncopyable {
	protected:
		noncopyable() = default;
		~noncopyable() = default;
	private:
		noncopyable(const noncopyable& that) = delete;
		noncopyable& operator=(const noncopyable& that) = delete;
	};

	class copyable {
	protected:
		copyable() = default;
		~copyable() = default;
	};
}