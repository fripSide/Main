#pragma once
#include "Render.h"
#include "Shader.hpp"
#include "World.h"
#include <GLFW/glfw3.h>

namespace GLDemo {

	class DemoRender : public Render {
	public:
		void onDrawFrame() override;
		void onSurfaceChanged(int width, int height) override;
		void onSurfaceCreated() override;
	    void onKeyEvent(int key_code) override;
		void onMouseEvent(double xpos, double ypos) override;
	private:
		GLDemo::World *world;

		// mouse event
		double lastX_;
		double lastY_;
		float delta_time;
		float last_frame_ti = 0;
	};
}
