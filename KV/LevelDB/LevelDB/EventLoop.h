#pragma once
#include "utils.h"
#include <thread>

namespace levdb {
	class EventLoop : noncopyable {
	public:
		EventLoop();
		~EventLoop();

		void loop();

		void assertInLoopThread();

		bool isInLoopThread() const;

	private:
		void abortNotInLoopThread();

		bool looping_;
		const size_t threadId_;
	};
}