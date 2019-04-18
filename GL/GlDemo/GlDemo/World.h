#pragma once
#include "camera.h"

/*
包括一个active_scene和其他的render target scene

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