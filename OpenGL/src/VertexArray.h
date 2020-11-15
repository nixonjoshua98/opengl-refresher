#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <memory>

class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& VBO, const VertexBufferLayout& layout);

	void AddBuffer(std::unique_ptr<VertexBuffer>& VBO, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

private:
	void AddBuffer(const VertexBufferLayout& layout);
};