#pragma once
#include "utils.h"
#include <functional>

namespace levdb {
	class EventLoop;
	class Channel : noncopyable {
	public:
		typedef std::function<void()> EventCallback;

	private:
		void update();

		EventLoop* loop_;
		const int fd_;
		int events_;
		int revents_;
		int index_;

		EventCallback readCallback_;
		EventCallback writeCallback_;
		EventCallback errorCallback_;
	};
}