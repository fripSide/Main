// LevelDB.cpp: 定义控制台应用程序的入口点。
//

/*
1. 学习C++基本语法
https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-introduction

2. leveldb源码学习
https://github.com/google/leveldb

TODO:
1. 实现reactor模式的网络框架
2. 实现基于protobuf的数据发送
3. 基于leveldb实现redis这样的单机kv
4. 实现分布式的kv
*/

#include "stdafx.h"
#include <iostream>
#include <memory>
#include "net/Socket.h"

void test_switch() {
	int t = 1;
	switch (t)
	{
	case 0:
		while (1) {
	case 2:
		;
		}

		break;
	default:
		break;
	}
}

// echo server
void test_socket() {
	levdb::Socket::Startup();
	levdb::TCPSocket sock;
	int sockfd = sock.Create();
	std::string url = "127.0.0.1";
	int port = sock.TryBindHost(url.data(), 9991, 9999);
	sock.sAddr = levdb::InetAddr(url.data(), port);
	sock.Listen();
	printf("Start to Listen %s:%d\n", sock.sAddr.AddrStr().data(), sock.sAddr.port());
	while (true) {
		levdb::TCPSocket client = sock.Accept();
		client.SendStr("hello client!");
		std::string msg;
		client.RecvStr(&msg);
		printf("recv string: %s\n", msg.data());
		client.Close();
	}
	
	levdb::Socket::Finalize();
}

int main()
{
	//创建db
	leveldb::DB * db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "tmp/testdb", &db);
	assert(status.ok());
	if (status.ok()) {
		std::cout << status.ToString() << std::endl;
	}
	std::unique_ptr<leveldb::DB> cur_db(db);

	//写操作
	const std::string  key = "name";
	std::string value = "fpeng";
	status = db->Put(leveldb::WriteOptions(), key, value);
	assert(status.ok());

	//读操作
	value.clear();
	status = db->Get(leveldb::ReadOptions(), key, &value);
	assert(status.ok());
	std::cout << key << ":" << value << std::endl;
	test_socket();
    return 0;
}

