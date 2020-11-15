
#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBufferLayout& layout)
{
	Bind();

	const auto& elements = layout.GetElements();

	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& ele = elements[i];

		glEnableVertexAttribArray(i);

		glVertexAttribPointer(i, ele.count, ele.type, ele.normalised, layout.GetStride(), (const void*)offset);

		offset += ele.count * VertexBufferElement::GetTypeSize(ele.type);
	}
}

void VertexArray::AddBuffer(const VertexBuffer& VBO, const VertexBufferLayout& layout)
{
	VBO.Bind();

	AddBuffer(layout);
}

void VertexArray::AddBuffer(std::unique_ptr<VertexBuffer>& VBO, const VertexBufferLayout& layout)
{
	VBO->Bind();

	AddBuffer(layout);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
