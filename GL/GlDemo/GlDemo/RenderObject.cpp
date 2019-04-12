#include "RenderObject.h"

using namespace GLDemo;

Mesh::Mesh(): Positions(0), UV(0), Normals(0) {
}


void Mesh::Init() {
	if (!m_VAO) {
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
	}

	// 分段放置数据
	std::vector<GLfloat> data;
	for (auto & p : Positions) {
		data.push_back(p.x);
		data.push_back(p.y);
		data.push_back(p.z);
	}
	for (auto & uv : UV) {
		data.push_back(uv.x);
		data.push_back(uv.y);
	}
	for (auto & nm : Normals) {
		data.push_back(nm.x);
		data.push_back(nm.y);
		data.push_back(nm.z);
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), &data[0], GL_STATIC_DRAW);

	if (Indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
	}

	size_t offset = 0, strip = 0;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strip, (void*) offset);
	offset += sizeof(GLfloat) * Positions.size();
	if (UV.size() > 0) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strip, (void*)offset);
		offset += sizeof(GLfloat) * UV.size();
	}

	if (Normals.size() > 0) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, strip, (void*)offset);
		offset += sizeof(GLfloat) * Normals.size();
	}

	glBindVertexArray(NULL);
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
	children_.push_back(child);
	child->parent_ = this;
}

void RenderObject::Draw() {
	DoDraw();
	for (auto child : children_) {
		child->Draw();
	}
}

Shader* Meterial::GetShader() {
	return m_Shader;
}

const std::vector<std::string> & Meterial::GetUnifromNames() {
	return {"mvp"};
}

void RenderObject::DoDraw() {
	if (!mtl_ || !mesh_) {
		return;
	}
	Shader *shader = mtl_->GetShader();
	shader->use();
	SetupUniforms();
	// 目前只画triangles
	mesh_->Draw();

	// TODO，支持画线和点，画出坐标轴和子变换
}

void RenderObject::SetTransfrom(const glm::mat4 &trans) {
	if (parent_) {
		transform_ = parent_->transform_ * transform_;
	}
	for (auto & child : children_) {
		child->SetTransfrom(trans);
	}
	transform_ = trans * transform_;
}

glm::mat4 RenderObject::GetTransfrom() {
	return transform_;
}

glm::vec3 RenderObject::GetLocalPostion() {
	return position_;
}

glm::vec3 RenderObject::GetWorldPostion() {
	glm::vec4 pos = transform_ * glm::vec4(position_, 1.0f);
	return glm::vec3(pos);
}

void RenderObject::SetPosition(const glm::vec3 &position) {
	position_ = position;
}

void RenderObject::SetupUniforms() {
	// set mvp
	auto mvp = GetTransfrom();
	mtl_->GetShader()->setMat4("mvp", mvp);
}