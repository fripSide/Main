#include "utils.h"
#include "MyFirstRender.h"
#include "DemoRender.h"
#include <iostream>

/*
TODO:
https://blog.csdn.net/wangdingqiaoit/article/details/51570001
ʹ�ÿ������3dѩ����ҵ��

��ǰ����Ļ�л������Ρ�
0. quad�ذ壬sqaureģ�ͣ�ball��fps���
1. ��������
2. ��Ӱ


�ܹ�����ͬ��pass�����ɵ�������rendertarget��Ȼ���ٺ�����Ļ��
render pass:
D:\Projects\sai\wing3d\main\source\core\renderer.cpp

TODO:
1. ����OpenGL��װ���
2. ʵ��LearnOpenGLȫ���Ľ̳�
3. Loadģ�͡�
4. ʵ�ֽű���

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

	//render.onSurfaceCreated();
	render->onSurfaceCreated();

	// main loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		//render.onDrawFrame();
		render->onDrawFrame();
		// process event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	render->onSurfaceChanged(width, height);
	render->onSurfaceChanged(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

}