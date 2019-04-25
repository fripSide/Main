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
		RenderTarget() {};
		RenderTarget(int width, int height);
		virtual ~RenderTarget();
		virtual void Init();
		void Use(GLenum type = GL_FRAMEBUFFER);
		
		void GenTexture(Texture &texture, GLenum inner_fmt, GLenum fmt, GLenum attach = GL_COLOR_ATTACHMENT0);
		void GenRenderBuffer(unsigned int &id, GLenum storage, GLenum attach);

		unsigned int fbo_;
		unsigned int buffer_id_;
		int width_;
		int hegiht_;

		Texture color_tex_;
		Texture motion_tex_;
	};

	class MotionBlurTarget : public RenderTarget {
	public:
		MotionBlurTarget(int w, int h);
		void Init() override;
	};
}