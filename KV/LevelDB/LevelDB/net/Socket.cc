#include "Socket.h"
#include "../utils.h"

namespace levdb {

	InetAddr::InetAddr(const char *url, int port) {
		this->Set(url, port);
	}

	std::string InetAddr::GetHostName(void) {
		std::string buf; buf.resize(256);
		utils::Check(gethostname(&buf[0], 256) != -1, "fail to get host name");
		return std::string(buf.c_str());
	}

	void InetAddr::Set(const char *host, int port) {
		hostent *hp = gethostbyname(host);
		utils::Check(hp != NULL, "cannot obtain address of %s", host);
		memset(&addr_, 0, sizeof(addr_));
		addr_.sin_family = AF_INET;
		addr_.sin_port = htons(port);
		memcpy(&addr_.sin_addr, hp->h_addr_list[0], hp->h_length);
	}

	int InetAddr::port() const {
		return ntohs(addr_.sin_port);
	}

	std::string InetAddr::AddrStr() const {
		std::string buf; buf.resize(256);
#ifdef _WIN32
		const char *s = inet_ntop(AF_INET, (PVOID)&addr_.sin_addr,
			&buf[0], buf.length());
#else
		const char *s = inet_ntop(AF_INET, &addr_.sin_addr,
			&buf[0], buf.length());
#endif
		utils::Assert(s != NULL, "cannot decode address");
		return std::string(s);
	}

	void Socket::SetNonBlock(bool non_block) {
#ifdef _WIN32
		u_long mode = non_block ? 1 : 0;
		if (ioctlsocket(sockfd_, FIONBIO, &mode) != NO_ERROR) {
			Socket::Error("SetNonBlock");
		}
#else
		int flag = fcntl(sockfd, F_GETFL, 0);
		if (flag == -1) {
			Socket::Error("SetNonBlock-1");
		}
		if (non_block) {
			flag |= O_NONBLOCK;
		}
		else {
			flag &= ~O_NONBLOCK;
		}
		if (fcntl(sockfd, F_SETFL, flag) == -1) {
			Socket::Error("SetNonBlock-2");
		}
#endif
	}

	void Socket::Startup() {
#ifdef _WIN32
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) == -1) {
			Socket::Error("Startup");
		}
		if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2) {
			WSACleanup();
			utils::Error("Could not find a usable version of Winsock.dll\n");
		}
#endif
	}

	void Socket::Finalize(void) {
#ifdef _WIN32
		WSACleanup();
#endif
	}

	void Socket::Error(const char *msg) {
		int errsv = GetLastError();
		utils::Error("Socket %s Error:WSAError-code=%d", msg, errsv);
	}

	void Socket::Bind(const InetAddr &addr) {
		if (bind(sockfd_, reinterpret_cast<const sockaddr*>(&addr.addr_),
			sizeof(addr.addr_)) == -1) {
			Socket::Error("Bind");
		}
	}

	int Socket::TryBindHost(const char * url, int start_port, int end_port) {
		for (int port = start_port; port < end_port; ++port) {
			InetAddr addr(url, port);
			if (bind(sockfd_, reinterpret_cast<sockaddr*>(&addr.addr_),
				sizeof(addr.addr_)) == 0) {
				return port;
			}
#if defined(_WIN32)
			if (WSAGetLastError() != WSAEADDRINUSE) {
				Socket::Error("TryBindHost");
			}
#else
			if (errno != EADDRINUSE) {
				Socket::Error("TryBindHost");
			}
#endif
		}

		return -1;
	}

	int Socket::GetSockError(void) const {
		int error = 0;
		socklen_t len = sizeof(error);
		if (getsockopt(sockfd_, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&error), &len) != 0) {
			Error("GetSockError");
		}
		return error;
	}

	bool Socket::BadSocket(void) const {
		if (IsClosed()) return true;
		int err = GetSockError();
		if (err == EBADF || err == EINTR) return true;
		return false;
	}

	void Socket::Close(void) {
		if (sockfd_ != INVALID_SOCKET) {
#ifdef _WIN32
			closesocket(sockfd_);
#else
			close(sockfd);
#endif
			sockfd_ = INVALID_SOCKET;
		}
		else {
			Error("Socket::Close double close the socket or close without create");
		}
	}

	int TCPSocket::Create(int af) {
		sockfd_ = socket(PF_INET, SOCK_STREAM, 0);
		if (sockfd_ == INVALID_SOCKET) {
			Socket::Error("Create");
		}
		return sockfd_;
	}

	void TCPSocket::Listen(int backlog) {
		listen(sockfd_, backlog);
	}

	void TCPSocket::SetKeepAlive(bool keepalive) {
		int opt = static_cast<int>(keepalive);
		if (setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
			reinterpret_cast<char*>(&opt), sizeof(opt)) < 0) {
			Socket::Error("SetKeepAlive");
		}
	}

	TCPSocket TCPSocket::Accept(void) {
		sockaddr_in addr;
		socklen_t t = sizeof addr;
		SOCKET newfd = accept(sockfd_, (sockaddr *)&addr, &t);
		if (newfd == INVALID_SOCKET) {
			Socket::Error("Accept");
		}

		TCPSocket tcpSocket(newfd);
		tcpSocket.sAddr.addr_ = addr;
		printf("Get Addr %s:%d\n", sAddr.AddrStr().data(), sAddr.port());
		return tcpSocket;
	}

	int TCPSocket::AtMark(void) const {
#ifdef _WIN32
		unsigned long atmark = 0;  // NOLINT(*)
		if (ioctlsocket(sockfd_, SIOCATMARK, &atmark) != NO_ERROR) return -1;
#else
		int atmark;
		if (ioctl(sockfd, SIOCATMARK, &atmark) == -1) return -1;
#endif
		return static_cast<int>(atmark);
	}

	bool TCPSocket::Connect(const InetAddr &addr) {
		return connect(sockfd_, reinterpret_cast<const sockaddr*>(&addr.addr_),
			sizeof(addr.addr_)) == 0;
	}

	ssize_t TCPSocket::Send(const void *buf_, size_t len, int flag) {
		const char *buf = reinterpret_cast<const char*>(buf_);
		return send(sockfd_, buf, static_cast<sock_size_t>(len), flag);
	}

	ssize_t TCPSocket::Recv(void *buf_, size_t len, int flags) {
		char *buf = reinterpret_cast<char*>(buf_);
		return recv(sockfd_, buf, static_cast<sock_size_t>(len), flags);
	}

	size_t TCPSocket::SendAll(const void *buf_, size_t len) {
		const char *buf = reinterpret_cast<const char*>(buf_);
		size_t ndone = 0;
		while (ndone < len) {
			ssize_t ret = send(sockfd_, buf, static_cast<ssize_t>(len - ndone), 0);
			if (ret == -1) {
				if (LastErrorWouldBlock()) return ndone;
				Socket::Error("SendAll");
			}
			buf += ret;
			ndone += ret;
		}
		return ndone;
	}

	size_t TCPSocket::RecvAll(void *buf_, size_t len) {
		char *buf = reinterpret_cast<char*>(buf_);
		size_t ndone = 0;
		while (ndone < len) {
			ssize_t ret = recv(sockfd_, buf,
				static_cast<sock_size_t>(len - ndone), MSG_WAITALL);
			if (ret == -1) {
				if (LastErrorWouldBlock()) return ndone;
				Socket::Error("RecvAll");
			}
			if (ret == 0) return ndone;
			buf += ret;
			ndone += ret;
		}
		return ndone;
	}

	void TCPSocket::SendStr(const std::string &str) {
		int len = static_cast<int>(str.length());
		utils::Assert(this->SendAll(&len, sizeof(len)) == sizeof(len),
			"error during send SendStr");
		if (len != 0) {
			utils::Assert(this->SendAll(str.c_str(), str.length()) == str.length(),
				"error during send SendStr");
		}
	}

	void TCPSocket::RecvStr(std::string *out_str) {
		int len;
		utils::Assert(this->RecvAll(&len, sizeof(len)) == sizeof(len),
			"error during send RecvStr");
		out_str->resize(len);
		if (len != 0) {
			utils::Assert(this->RecvAll(&(*out_str)[0], len) == out_str->length(),
				"error during send SendStr");
		}
	}
}
