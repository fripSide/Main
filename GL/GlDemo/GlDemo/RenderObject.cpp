#include "RenderObject.h"

using namespace GLDemo;

Mesh::Mesh(): Positions(0), UV(0), Normals(0) {
}

void Mesh::Draw() {
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (Indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glDrawElements(drawMode, Indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(drawMode, 0, Positions.size());
	}
}

void RenderObject::AddChild(RenderObject *child) {
	m_Children.push_back(child);
	child->parent = this;
}

void RenderObject::Draw() {

	DoDraw();
	for (auto child : m_Children) {
		child->Draw();
	}
}

Shader* Meterial::GetShader() {
	return m_Shader;
}

void RenderObject::DoDraw() {
	if (!mtl || !mesh) {
		return;
	}
	Shader *shader = mtl->GetShader();
	shader->use();
	mesh->Draw();
}