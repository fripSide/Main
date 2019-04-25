#include "utils.h"
#include <iostream>

const bool DEBUG = true;

void InitOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void DumpMat4(glm::mat4 mt) {
	printf("--------------\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%f ", mt[i][j]);
		}
		printf("\n");
	}
	printf("--------------\n");
}

void Log(const char *fmt, ...) {
	if (!DEBUG)
		return;
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	fflush(stdout);
	va_end(ap);
}