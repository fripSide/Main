#include "EventLoop.h"
#include <chrono>

namespace levdb {
	thread_local EventLoop* t_loopInThisThread = 0;

	EventLoop::EventLoop() : looping_(false), threadId_(utils::GetThreadId(std::this_thread::get_id())) {
		utils::Log("EventLoop created %d in thread %zu\n", this, threadId_);
		if (t_loopInThisThread) {
			utils::Log("Another EventLoop %lld exists in this thread %zu", this, threadId_);
		}
		else {
			t_loopInThisThread = this;
		}
	}

	EventLoop::~EventLoop() {
		assert(!looping_);
		t_loopInThisThread = NULL;
	}

	void EventLoop::assertInLoopThread() {
		if (!isInLoopThread()) {
			abortNotInLoopThread();
		}
	}

	void EventLoop::loop() {
		assert(!looping_);
		assertInLoopThread();
		looping_ = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		printf("EventLoop loop %zu\n", threadId_);
		looping_ = false;
	}

	bool EventLoop::isInLoopThread() const {
		return threadId_ == utils::GetThreadId(std::this_thread::get_id());
	}

	void EventLoop::abortNotInLoopThread() {
		utils::Log("EventLoop::abortNotInLoopThread - EventLoop %d exists in this thread %zu\n", this, threadId_);
	}
}