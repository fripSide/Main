#pragma once
#include "RenderTarget.h"
#include "RenderObject.h"

namespace GLDemo {
	//http://ogldev.atspace.co.uk/www/tutorial41/tutorial41.html

	// TODO
	// 将所有的pass组织在这里
	class RenderPass {
	public:
		RenderPass();
		void Culling();
		void PassNormal();
		void RenderPipeline();
		
		void SetScreenSize(int w, int h);

		void OnRenderImage(RenderTarget *src, RenderTarget *dest);

		void RenderToScreen();

		void Setup();

		RenderTarget *post_effect_buffer_ = NULL;
		RenderTarget *screen_ = NULL;
		RenderObject *screen_quad_;
		bool use_post_process_ = true;
	};
}