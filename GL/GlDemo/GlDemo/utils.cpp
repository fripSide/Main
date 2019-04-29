#include "utils.h"
#include <iostream>
#include <glad/glad.h>

const bool DEBUG = true;

void InitOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (GL_DEBUG) {
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}
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

GLenum glCheckError_(const char *file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		Log("GLError: %s | %s (%d)\n", error.data(), file, line);
	}
	return errorCode;
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam) {
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	Log("---------------\n");
	Log("Debug message (%d) %s\n", id, message);
	std::string err;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             err = "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   err = "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: err = "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     err = "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     err = "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           err = "Source: Other"; break;
	}
	Log("%s\n", err.data());

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               err = "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: err = "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  err = "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         err = "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         err = "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              err = "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          err = "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           err = "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               err = "Type: Other"; break;
	} 
	Log("%s\n", err.data());

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         err = "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       err = "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          err = "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: err = "Severity: notification"; break;
	}
	Log("%s\n\n", err.data());
}

void CheckToEnableGLDebug() {
	if (!GL_DEBUG) {
		return;
	}
	//glad_set_post_callback(_post_call_callback_default);
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}

void _post_call_callback_default(const char *name, void *funcptr, int len_args, ...) {
	GLenum error_code;
	error_code = glad_glGetError();

	if (error_code != GL_NO_ERROR) {
		fprintf(stderr, "ERROR %d in %s\n", error_code, name);
	}
}
