#pragma once
#include "Shader.hpp"
#include <vector>
#include <glad/glad.h> 

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



	class Meterial {
	protected:
		Shader *m_Shader;
	public:
		Shader* GetShader();
		const std::vector<std::string> & GetUnifromNames();

	private:
		std::vector<std::string> unfirom_names_;
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
		void Draw();
		void Init();
	};

	class RenderObject {
	public:
		Mesh *mesh_;
		Meterial *mtl_;

		RenderObject *parent_;
		std::vector<RenderObject*> children_;

	public:
		void AddChild(RenderObject *child);
		void Draw();
		void SetTransfrom(const glm::mat4 &trans);
		glm::mat4 GetTransfrom();
		void SetPosition(const glm::vec3 &position);
		glm::vec3 GetLocalPostion();
		glm::vec3 GetWorldPostion();

	private:
		void DoDraw();
		void SetupUniforms();

	private:
		glm::mat4 transform_ = glm::mat4(1);
		glm::vec3 position_= glm::vec3(0);
		bool dirty_;
	};
}