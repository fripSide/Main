#pragma once
#include "camera.h"
#include "RenderPass.h"
#include <string>

/*
包括一个active_scene和其他的render target scene

*/
namespace GLDemo {
	class RenderObject;
	class RenderPass;

	class World {
	private:
		static World *instance_;
		World();
		~World();
		
		RenderObject *root;
		void SetUnifromByName(RenderObject *node, const std::string &name);

	public:
		static World* Instance();
		void SetScreenSize(int w, int h);
		void Update();
		void AddChildNode(RenderObject *node);
		void SetSceneUniforms(RenderObject *node);
		RenderObject* GetAtiveScene();

	public:
		Camera mainCamera_;
		RenderPass render_pass_;

		int width_;
		int height_;
	};
}