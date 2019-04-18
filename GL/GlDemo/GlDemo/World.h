#pragma once
#include "camera.h"

/*
����һ��active_scene��������render target scene

*/
namespace GLDemo {
	class RenderObject;
	class Camera;

	class World {
	private:
		static World *instance_;
		World();
		~World();
		RenderObject *root;
		

	public:
		static World* Instance();
		void Update();
		void AddChildNode(RenderObject *node);

	public:
		Camera mainCamera_;
	};
}