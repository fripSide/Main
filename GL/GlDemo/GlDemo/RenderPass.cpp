#include "RenderPass.h"
#include "World.h"
#include "RenderObject.h"
#include "utils.h"
#include "SceneItems.h"

using namespace GLDemo;

RenderPass::RenderPass(): screen_quad_(new ScreenQuad) {
}

void RenderPass::RenderPipeline() {
	Culling();
	if (use_post_process_) {
		//post_effect->Use();
		screen_->Use();
	}
	PassNormal();
	if (use_post_process_) {
		OnRenderImage(post_effect_, screen_);
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
*/
void RenderPass::PassNormal() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render to 
	World::Instance()->GetAtiveScene()->Draw();
}

void RenderPass::RenderToScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screen_quad_->mtl_->SetTexture("screenTexture", &screen_->target);
	screen_quad_->Draw();
}

void RenderPass::SetScreenSize(int w, int h) {
	if (screen_ == NULL) {
		screen_ = new RenderTarget(w, h);
		post_effect_ = new RenderTarget(w, h);
	}
	else if (screen_->width_ != w || screen_->hegiht_ != h) {
		delete screen_;
		delete post_effect_;
		screen_ = new RenderTarget(w, h);
		post_effect_ = new RenderTarget(w, h);
	}
}