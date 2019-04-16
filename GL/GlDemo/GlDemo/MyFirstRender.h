#pragma once
#include "Render.h"
#include "Shader.hpp"

using namespace GLDemo;
class MyFirstRender : public Render {
public:
	void onDrawFrame() override;
	void onSurfaceChanged(int width, int height) override;
	void onSurfaceCreated() override;
private:
	Shader s1;
	unsigned int VBO, VAO, EBO;
};