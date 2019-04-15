#pragma once

/*
包括一个active_scene和其他的render target scene

*/
namespace GLDemo {
	class RenderObject;

	class World {
	private:
		RenderObject *root;
	
	public:
		World();
		void Update();

		void AddChildNode(RenderObject *node);
	};
}