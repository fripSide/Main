#pragma once
#include "Render.h"
#include "Shader.hpp"
#include "World.h"

namespace GLDemo {
	class DemoRender : public Render {
	public:
		void onDrawFrame() override;
		void onSurfaceChanged(int width, int height) override;
		void onSurfaceCreated() override;
		void onSurfaceCreated() override;
	    void onKeyEvent(GLFWwindow *window) override;
		void onMouseEvent(double xpos, double ypos) override;
	private:
		GLDemo::World world;

		// mouse event
		double lastX_;
		double lastY_;

	};
}
