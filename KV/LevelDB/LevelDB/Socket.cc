#include "stdafx.h"
#include "Socket.h"
#include "utils.h"

namespace levdb {

	InetAddr::InetAddr(const char *url, int port) {

	}

	std::string InetAddr::GetHistName(void) {
		std::string buf; buf.resize(256);
		utils::Check(gethostname(&buf[0], 256) != -1, "fail to get host name");
		return std::string(buf.c_str());
	}

	void InetAddr::Set(const char *host, int port) {
		hostent *hp = gethostbyname(host);
		utils::Check(hp != NULL, "cannot obtain address of %s", host);

	}
}
