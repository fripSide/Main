#pragma once
#include "Texture.h"

namespace GLDemo {
	/*
	帧缓冲：包括若干个texture，1个RenderBuffer
	texture：纹理附件，输出。输出数应该和shader种的out数一致。
	RenderBuffer： 渲染缓冲对象，保存depth,stencil等结果
	*/
	class RenderTarget {
	public:
		RenderTarget(int width, int height);
		virtual ~RenderTarget();
		virtual void Init();
		virtual void Use(GLenum type = GL_FRAMEBUFFER);
		
		void GenTexture(Texture &texture, bool depth, bool stencil);
		void GenRenderBuffer(unsigned int &id, GLenum storage, GLenum attach);

		unsigned int fbo_;
		unsigned int buffer_id_;
		int width_;
		int hegiht_;

		Texture target;
	};
}