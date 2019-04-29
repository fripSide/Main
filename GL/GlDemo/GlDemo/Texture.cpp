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
	if (id_ == -1) return;
	if (unit >= 0) {
		glActiveTexture(GL_TEXTURE0 + unit);
	}
	glBindTexture(target_, id_);
}
void Texture::UnBind() {
	glBindTexture(target_, 0);
}

CubeMap* CubeMap::LoadCubeMap(const std::vector<std::string> &faces) {
	CubeMap *tex = new CubeMap;
	glGenTextures(1, &tex->id_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex->id_);
	int width, height, nrChannels;
	for (int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			Log("Cubemap texture failed to load:%s\n", faces[i].c_str());
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return tex;
}

void CubeMap::Bind(int unit) {
	if (id_ == -1) return;
	if (unit >= 0)
		glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
}

void CubeMap::UnBind() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
