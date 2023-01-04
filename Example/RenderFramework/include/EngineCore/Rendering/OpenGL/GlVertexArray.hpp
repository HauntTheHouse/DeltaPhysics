#pragma once

#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "GlTypes.hpp"

class GlVertexArray
{
public:
	GlVertexArray() = default;
	~GlVertexArray() = default;
	GlVertexArray(GlVertexArray&) = delete;
	GlVertexArray& operator=(GlVertexArray&) = delete;

	bool init();
	void clear();

	void addVertexBuffer(const GlVertexBuffer& aVertexBuffer);
	void setIndexBuffer(const GlIndexBuffer& aIndexBuffer);

	void bind() const;
	static void unbind();

	GLuint getVerticesCount() { return mVerticesCount; }
	GLsizei getIndicesCount() { return mIndicesCount; }

private:
	GLuint mId{ 0 };

	GLuint mAttributesCount{ 0 };
	GLsizei mVerticesCount{ 0 };
	GLsizei mIndicesCount{ 0 };
};