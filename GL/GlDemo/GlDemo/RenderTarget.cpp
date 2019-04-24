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
	GenTexture(target, false, false);
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

void RenderTarget::GenTexture(Texture &texture, bool depth, bool stencil) {
	GLenum attach_type = GL_RGB;
	if (!depth && !stencil) {
		attach_type = GL_RGB;
	}
	else if (depth && !stencil) {
		attach_type = GL_DEPTH_COMPONENT;
	}
	else if (!depth && stencil) {
		attach_type = GL_STENCIL_INDEX;
	}

	texture.filterMin_ = GL_NEAREST;
	texture.filterMax_ = GL_NEAREST;
	texture.wrapS_ = GL_TEXTURE_MIN_FILTER;
	texture.wrapT_ = GL_TEXTURE_MAG_FILTER;
	texture.use_mipmap_ = false;
	if (depth && stencil) {
		texture.GenGLTexture2D(width_, hegiht_, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	}
	else {
		texture.GenGLTexture2D(width_, hegiht_, attach_type, attach_type, GL_UNSIGNED_BYTE, NULL);
	}
	
	// attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_, 0);
}

void RenderTarget::GenRenderBuffer(unsigned int &id, GLenum storage, GLenum attach) {
	glGenRenderbuffers(1, &id);
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, storage, width_, hegiht_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attach, GL_RENDERBUFFER, id);
}