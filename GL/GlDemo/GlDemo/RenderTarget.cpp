#include "RenderTarget.h"
#include "utils.h"

using namespace GLDemo;

RenderTarget::RenderTarget(int w, int h) : width_(w), hegiht_(h) {
	glGenFramebuffers(1, &fbo_);
	Init();
}

RenderTarget::~RenderTarget() {
	glDeleteFramebuffers(1, &fbo_);
}

// 默认创建一个纹理，并开启深度测试和模板测试
void RenderTarget::Init() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	GenTexture(color_tex_, GL_RGB, GL_RGB);
	GenRenderBuffer(buffer_id_, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::Use(GLenum type) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	glEnable(GL_DEPTH_TEST);
}

void RenderTarget::GenTexture(Texture &texture, GLenum inner_fmt, GLenum fmt, GLenum attach) {
	texture.filterMin_ = GL_NEAREST;
	texture.filterMax_ = GL_NEAREST;
	texture.wrapS_ = GL_CLAMP_TO_EDGE;
	texture.wrapT_ = GL_CLAMP_TO_EDGE;
	texture.use_mipmap_ = false;
	texture.GenGLTexture2D(width_, hegiht_, inner_fmt, fmt, GL_FLOAT, NULL);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, texture.id_, 0);
}

void RenderTarget::GenRenderBuffer(unsigned int &id, GLenum storage, GLenum attach) {
	glGenRenderbuffers(1, &id);
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, storage, width_, hegiht_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attach, GL_RENDERBUFFER, id);
}

MotionBlurTarget::MotionBlurTarget(int w, int h) {
	width_ = w;
	hegiht_ = h;
	glGenFramebuffers(1, &fbo_);
	Init();
}

//void MotionBlurTarget::Use() {
//	glActiveTexture(COLOR_TEXTURE_UNIT);
//}

void MotionBlurTarget::Init() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	GenTexture(color_tex_, GL_RGBA, GL_RGBA, GL_COLOR_ATTACHMENT0);
	GenTexture(motion_tex_, GL_RGBA, GL_RGBA, GL_COLOR_ATTACHMENT1);
	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);
	/*glActiveTexture(0);
	glActiveTexture(1);*/
	GenRenderBuffer(buffer_id_, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}