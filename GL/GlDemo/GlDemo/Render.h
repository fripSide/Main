#pragma once
/*
TODO: 默认android opengles的封装，来实现一个简单的框架
https://www.jianshu.com/p/4a014afde409
*/

class Render {
public:
	virtual ~Render() {};
	virtual void onDrawFrame() = 0;
	virtual void onSurfaceChanged(int width, int height) = 0;
	virtual void onSurfaceCreated() = 0;
protected:
	Render() {};
};