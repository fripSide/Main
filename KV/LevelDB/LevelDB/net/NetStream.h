#pragma once
#include <vector>

namespace levdb {
	namespace net {
		/*
		TODO：
		1. 先实现简单粗暴的线性buffer,通过shrink将内容前移。
		2. 实现ring buffer
		*/
		class Buffer {
		public:
			static const size_t kInitialSize = 1024;


		private:
			std::vector<char> buffer_;
			size_t readerIndex_;
			size_t writerIndex_;
		};
	}
}