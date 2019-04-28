#include "RenderPass.h"
#include "World.h"
#include "RenderObject.h"
#include "utils.h"
#include "SceneItems.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace GLDemo;

RenderPass::RenderPass() {
	//screen_quad_ = new ScreenQuad;
	screen_quad_ = new RenderObject;
	screen_quad_->mesh_ = new ScreenMesh;
	screen_quad_->SetTransfrom(glm::scale(glm::mat4(), { 2, 2, 1 }));
	screen_quad_->mtl_ = new ShaderOnlyMaterial("shader/motion_blur.vs", "shader/motion_blur.fs");
}

void RenderPass::Setup() {
	screen_quad_->mtl_->SetTexture("screenTexture", &screen_->color_tex_, 0);
	screen_quad_->mtl_->SetTexture("motionTexture", &screen_->motion_tex_, 1);
}

void RenderPass::RenderPipeline() {
	Culling();
	if (use_post_process_) {
		screen_->Use();
	}
	PassNormal();
	if (use_post_process_) {
		OnRenderImage(post_effect_buffer_, screen_);
		RenderToScreen();
	}
}

void RenderPass::Culling() {

}

// 所有的后处理效果在这里实现
void RenderPass::OnRenderImage(RenderTarget *src, RenderTarget *dest) {
	
}

/*
后处理
https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/05%20Framebuffers/

利用卷积实现屏幕效果
https://blog.csdn.net/fansongy/article/details/79263735
*/
void RenderPass::PassNormal() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render to 
	World::Instance()->GetAtiveScene()->Draw();
}

void RenderPass::RenderToScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screen_quad_->Draw();
}

void RenderPass::SetScreenSize(int w, int h) {
	if (screen_ != NULL && (screen_->width_ != w || screen_->hegiht_ != h)) {
		delete screen_;
		delete post_effect_buffer_;	
		screen_ = NULL;
	}

	if (screen_ == NULL) {
		//screen_ = new RenderTarget(w, h);
		screen_ = new MotionBlurTarget(w, h);
		post_effect_buffer_ = new RenderTarget(w, h);
		Setup();
	}
}