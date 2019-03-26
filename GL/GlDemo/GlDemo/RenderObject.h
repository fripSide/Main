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
	};

	class RenderObject {
	public:
		Mesh *mesh;
		Meterial *mtl;

		RenderObject *parent;
		std::vector<RenderObject*> m_Children;

	public:
		void AddChild(RenderObject *child);
		void Draw();

	private:
		void DoDraw();
	};
}