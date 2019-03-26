#pragma once

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