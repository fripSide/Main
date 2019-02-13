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

		static std::string GetHostName(void);

		void Set(const char *host, int port);

		int port() const;

		std::string AddrStr() const;
		
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

		static void Startup(void);

		static void Finalize(void);

		/*! \return whether last error was would block */
		static bool LastErrorWouldBlock(void) {
			int errsv = GetLastError();
#ifdef _WIN32
			return errsv == WSAEWOULDBLOCK;
#else
			return errsv == EAGAIN || errsv == EWOULDBLOCK;
#endif
		}

		static void Error(const char *msg);

		void SetNonBlock(bool non_block);

		void Bind(const InetAddr &addr);

		int TryBindHost(const char * url, int start_port, int end_port);

		int GetSockError(void) const;

		bool BadSocket(void) const;

		bool IsClosed(void) const {
			return sockfd_ == INVALID_SOCKET;
		}

		void Close(void);

	};

	class TCPSocket : public Socket {
	public:
		TCPSocket(void) : Socket(INVALID_SOCKET) {}
		explicit TCPSocket(SOCKET sockfd) : Socket(sockfd) {
		}

		int Create(int af = PF_INET);

		void SetKeepAlive(bool keepalive);

		void Listen(int backlog = 16);

		TCPSocket Accept(void);

		/*!
		* \brief decide whether the socket is at OOB mark
		* \return 1 if at mark, 0 if not, -1 if an error occured
		*/
		int AtMark(void) const;

		bool Connect(const InetAddr &addr);

		ssize_t Send(const void *buf_, size_t len, int flag = 0);

		ssize_t Recv(void *buf_, size_t len, int flags = 0);

		size_t SendAll(const void *buf_, size_t len);

		size_t RecvAll(void *buf_, size_t len);

		void SendStr(const std::string &str);

		void RecvStr(std::string *out_str);

	public:
		InetAddr sAddr;
	};
}