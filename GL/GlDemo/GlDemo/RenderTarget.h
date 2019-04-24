#pragma once
#include "Texture.h"

namespace GLDemo {
	/*
	֡���壺�������ɸ�texture��1��RenderBuffer
	texture��������������������Ӧ�ú�shader�ֵ�out��һ�¡�
	RenderBuffer�� ��Ⱦ������󣬱���depth,stencil�Ƚ��
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