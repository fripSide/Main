#pragma once

class RenderObject;

namespace GLDemo {

	class World {
	private:
		RenderObject *root;
	
	public:
		void Update();
	};
}