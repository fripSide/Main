#pragma once
#include "Shader.hpp"
#include <vector>
#include <glad/glad.h> 
#include <map>
#include "Texture.h"

namespace GLDemo {

	enum TOPOLOGY
	{
		POINTS,
		LINES,
		LINE_STRIP,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
	};


	/*
	将Unifroms参数保存到material中，在渲染器修改。
	shader.use()之后需要再设置一次uniforms
	*/
	class Material {
	protected:
		Shader *shader_;
	public:
		Material();
		Shader* GetShader();
		const std::vector<std::string> & GetUnifromNames();
		void SetTexture(std::string name, Texture *val, unsigned int unit = 0);
		void SetCubeMap(std::string name, CubeMap *cube, unsigned int unit = 0);
		void SetupUniforms();
		std::vector<std::string> scene_uniform_names_;

	protected:
		
		std::map<std::string, UniformValue> uniforms_;
		// textures
		std::map<std::string, UniformTextureSampler> samplerUniforms_;
	};

	class Mesh {
	public:
		unsigned int m_VAO = 0;
		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		std::vector<glm::vec3> Positions;
		std::vector<glm::vec2> UV;
		std::vector<glm::vec3> Normals;
		std::vector<unsigned int> Indices;
		GLenum drawMode = GL_TRIANGLES;

	public:
		Mesh();
		virtual void SetGL() {};
		virtual void ResetGL() {};
		void Draw();
		void Init();
	};

	class RenderObject {
	public:
		Mesh *mesh_;
		Material *mtl_;

		RenderObject *parent_;
		std::vector<RenderObject*> children_;

		glm::mat4 prev_trans_ = glm::mat4(1);
		glm::mat4 transform_ = glm::mat4(1);

	public:
		void AddChild(RenderObject *child);
		void Draw();
		void SetTransfrom(const glm::mat4 &trans);
		//glm::mat4 GetTransfrom();
		void SetPosition(const glm::vec3 &position);
		glm::vec3 GetLocalPostion();
		glm::vec3 GetWorldPostion();
		glm::mat4 GetTransfrom();

	protected:
		virtual void DoDraw();
		void SetupUniforms();
	

	private:
		
		glm::vec3 position_= glm::vec3(0);
		bool dirty_ = false;
	};
}