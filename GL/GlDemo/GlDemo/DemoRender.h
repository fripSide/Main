#pragma once
#include "Render.h"
#include "Shader.hpp"
#include "World.h"

class DemoRender : public Render {
public:
	void onDrawFrame() override;
	void onSurfaceChanged(int width, int height) override;
	void onSurfaceCreated() override;
private:
	GLDemo::World world;
};