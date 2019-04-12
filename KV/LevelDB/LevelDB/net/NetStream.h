#pragma once
#include <vector>

namespace levdb {
	namespace net {
		/*
		TODO��
		1. ��ʵ�ּ򵥴ֱ�������buffer,ͨ��shrink������ǰ�ơ�
		2. ʵ��ring buffer
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