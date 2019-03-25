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
	};

	class Mesh {
	public:
		unsigned int m_VAO = 0;
		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		std::vector<glm::vec3> Positions;
		std::vector<glm::vec2> UV;
		std::vector<glm::vec3> Normals;

	public:
		void Draw();
	};

	class RenderObject {
	public:
		Mesh *mesh;
		Meterial *mtl;


	private:
		RenderObject *parent;
		std::vector<RenderObject*> m_Children;
	};
}