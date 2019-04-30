#include "utils.h"
#include "MyFirstRender.h"
#include "DemoRender.h"
#include <iostream>

/*
TODO:
https://blog.csdn.net/wangdingqiaoit/article/details/51570001
使用框架重做3d雪人作业。

当前：屏幕中画三角形。
0. quad地板，sqaure模型，ball球，fps相机
1. 基本光照
2. 阴影


架构，不同的pass，生成到单独的rendertarget，然后再后处理到屏幕。
render pass:
D:\Projects\sai\wing3d\main\source\core\renderer.cpp

TODO:
1. 基于OpenGL封装框架
2. 实现LearnOpenGL全部的教程
3. Load模型。
4. 实现脚本。

*/

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Render *render;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main() {
	//render = new MyFirstRender;
	render = new DemoRender;
	// opengl init
	InitOpenGL();

	// create window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//glEnable(GL_DEPTH_TEST);

	CheckToEnableGLDebug();

	render->onSurfaceCreated();
	
	// main loop
	while (!glfwWindowShouldClose(window)) {
		glCheckError();
		processInput(window);
		render->onDrawFrame();
		// process event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// 暂时只考虑同时按一个键
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	int key = 0;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		key = GLFW_KEY_ESCAPE;
	}
	int key_list[] = {
		GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D,
		GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_UP
	};
	for (auto k : key_list) {
		if (glfwGetKey(window, k) == GLFW_PRESS) {
			render->onKeyEvent(k);
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	render->onSurfaceChanged(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	render->onMouseEvent(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	
}