// LevelDB.cpp: 定义控制台应用程序的入口点。
//

/*
1. 学习C++基本语法
https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-introduction

2. leveldb源码学习
https://github.com/google/leveldb

TODO:
1. 实现reactor模式的网络框架
学习 Eil Bendersky的教程并做笔记
https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/
2. 实现基于protobuf的数据发送
3. 基于leveldb实现redis这样的单机kv
4. 实现分布式的kv

当前进度：
01，https://github.com/chenshuo/recipes/tree/master/reactor

*/

#include "stdafx.h"
#include <iostream>
#include <memory>
#include "net/Socket.h"
#include "EventLoop.h"

using namespace levdb;

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

void test_reactor() {
	printf("main(): pid = %d, tid = %zu\n", _getpid(), utils::GetThreadId(std::this_thread::get_id()));

	levdb::EventLoop loop;

	std::thread th1([&] {
		levdb::EventLoop lp2;
		lp2.loop();
		//loop.loop();
	});

	loop.loop();
	th1.join();
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
	std::string value = "snk";
	status = db->Put(leveldb::WriteOptions(), key, value);
	assert(status.ok());

	//读操作
	value.clear();
	status = db->Get(leveldb::ReadOptions(), key, &value);
	assert(status.ok());
	std::cout << key << ":" << value << std::endl;

	test_reactor();
	//test_socket();
    return 0;
}

