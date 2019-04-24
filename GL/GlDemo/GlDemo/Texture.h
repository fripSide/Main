#pragma once
#include <string>
#include "stb_image.h"
#include <glad/glad.h>
#include <vector>

namespace GLDemo {
	class Texture {
	public:
		unsigned int id_;
		GLenum target_ = GL_TEXTURE_2D;
		GLenum internalFormat_ = GL_RGBA;
		GLenum format_ = GL_RGBA;
		GLenum data_type_ = GL_UNSIGNED_BYTE;
		unsigned int width_ = 0;
		unsigned int height_ = 0;
		unsigned int depth_ = 0;
		//https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/06%20Textures/
		// �������
		/*
		ע������������ѡ�� GL_LINEAR_MIPMAP_LINEAR����ô����һ��Ҫ����mipmap����Ȼ�ͻ᲻��ʾ����
		*/
		GLenum filterMin_ = GL_LINEAR_MIPMAP_LINEAR;
		GLenum filterMax_ = GL_LINEAR; // ȡ�ڽ��������Ի��       
		// �ظ���ʽ 
		GLenum wrapS_ = GL_REPEAT; // x           
		GLenum wrapT_ = GL_REPEAT; // y          
		GLenum wrapR_ = GL_REPEAT; // z          
		bool use_mipmap_ = true;

	public:
		static Texture* LoadTexture(std::string path, GLenum target = GL_TEXTURE_2D, GLenum internalFormat = GL_RGBA, bool srgb = false);

		void Bind(int unit = 0);
		void UnBind();

		void SetWrapMode(GLenum wrapMode, bool bind = false);

		void GenGLTexture2D(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, void* data);
	};

	class CubeMap {
	public:
		unsigned int id_;

		void Bind(int unit = 0);
		void UnBind();


		static CubeMap* LoadCubeMap(const std::vector<std::string> & faces);
	};
}