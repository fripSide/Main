// LevelDB.cpp: 定义控制台应用程序的入口点。
//

/*
1. 学习C++基本语法
https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-introduction

2. leveldb源码学习
https://github.com/google/leveldb
*/

#include "stdafx.h"
#include <iostream>
#include <memory>

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
    return 0;
}

