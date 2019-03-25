#pragma once
#include "Render.h"
#include "Shader.hpp"

class DemoRender : public Render {
public:
	void onDrawFrame() override;
	void onSurfaceChanged(int width, int height) override;
	void onSurfaceCreated() override;
private:
	
};