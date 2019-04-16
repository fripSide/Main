#include "Texture.h"
#include "utils.h"

using namespace GLDemo;

Texture * Texture::LoadTexture(std::string path, GLenum target, GLenum internalFormat, bool srgb) {
	Texture * tex = new Texture;
	tex->target_ = target;
	tex->internalFormat_ = internalFormat;

	int width, height, nChannels;
	unsigned char *data = stbi_load(path.data(), &width, &height, &nChannels, 0);
	//GenGLTexture2D(wi)
	if (data) {
		GLenum res_fmt;
		if (nChannels == 1) {
			res_fmt = GL_RED;
		}
		else if (nChannels == 3) {
			res_fmt = GL_RGB;
		}
		else if (nChannels == 4) {
			res_fmt = GL_RGBA;
		}
		if (target == GL_TEXTURE_1D) {

		}
		else if (target == GL_TEXTURE_2D) {
			tex->GenGLTexture2D(width, height, tex->internalFormat_, res_fmt, GL_UNSIGNED_BYTE, data);
		}
		stbi_image_free(data);
		tex->width_ = width;
		tex->height_ = height;
		return tex;
	}
	else {
		Log("Failed to load Texture at path: %s\n", path.data());
		stbi_image_free(data);
		return NULL;
	}
}

/*
注意必须生成mipmap
*/
void Texture::GenGLTexture2D(unsigned int width, unsigned int height, GLenum internalFormat, GLenum res_fmt, GLenum type, void* data) {
	glGenTextures(1, &id_);
	width_ = width;
	height_ = 0;
	depth_ = 0;
	internalFormat_ = internalFormat;
	format_ = res_fmt;
	data_type_ = type;

	//glActiveTexture(GL_TEXTURE0);
	Bind();
	glTexImage2D(target_, 0, internalFormat, width, height, 0, res_fmt, type, data);
	glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, filterMin_);
	glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, filterMax_);
	glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrapS_);
	glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrapT_);
	if (use_mipmap_) {
		glGenerateMipmap(target_);
	}
	UnBind();
}

void Texture::Bind(int unit) {
	if (unit >= 0) {
		glActiveTexture(GL_TEXTURE0 + unit);
	}
	glBindTexture(target_, id_);
}
void Texture::UnBind() {
	glBindTexture(target_, 0);
}