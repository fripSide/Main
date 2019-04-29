#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// debug
#define GLAD_DEBUG
const bool GL_DEBUG = true;

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

void InitOpenGL();

void DumpMat4(glm::mat4 mt);

void Log(const char *fmt, ...);

GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

void CheckToEnableGLDebug();