#pragma once
/*
TODO: Ĭ��android opengles�ķ�װ����ʵ��һ���򵥵Ŀ��
https://www.jianshu.com/p/4a014afde409
*/
class GLFWwindow;
namespace GLDemo {
	class Render {
	public:
		virtual ~Render() {};
		virtual void onDrawFrame() = 0;
		virtual void onSurfaceChanged(int width, int height) = 0;
		virtual void onSurfaceCreated() = 0;
		virtual void onMouseEvent(double xpos, double ypos) = 0;
		virtual void onKeyEvent(GLFWwindow *window) = 0;
	protected:
		Render() {};
	};
}
