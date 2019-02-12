#pragma once

#if defined(_WIN32)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif

#else

#endif

#if defined(_WIN32)
typedef int ssize_t;
typedef int sock_size_t;
#else
typedef int SOCKET;
typedef size_t sock_size_t;
const int INVALID_SOCKET = -1;
#endif

#include <string>

namespace levdb {
	class InetAddr {
	public:
		InetAddr() {}
		InetAddr(const char *url, int port);

		static std::string GetHistName(void);

		void Set(const char *host, int port);

		int port() const;

		std::string AddrStr() const;
		
	private:
		union
		{
			struct sockaddr_in addr_;
			struct sockaddr_in6 addr6_;
		};
	};

	class Socket {
	protected:
		explicit Socket(SOCKET sockfd) : sockfd_(sockfd) {
		}
	public:
		SOCKET sockfd_;

		int fd() const { return sockfd_; };

		static int GetLastError(void) {
#ifdef _WIN32
			return WSAGetLastError();
#else
			return 0;
#endif
		}
	};

	class TcpSocket : public Socket {

	};
}