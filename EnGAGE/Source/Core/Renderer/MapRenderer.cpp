#include "pch.hpp"
#include "MapRenderer.hpp"

#include "Core/Map/Map.hpp"
#include "Core/Math.hpp"

#include <glad/glad.h>

Core::MapRenderer::~MapRenderer()
{
	mShader.cleanup();
}

void Core::MapRenderer::render(const Camera& camera)
{
	mShader.bind();
	mShader.uploadProjView(Math::calculateProjectionView(camera));
	
	auto& mesh = Map::getData().wallMesh;
	
	if (mesh.textureSheet != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.textureSheet->id);
	}
	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
	glBindVertexArray(0);
}

void Core::MapRenderer::renderNoShader()
{
	auto& mesh = Map::getData().wallMesh;
	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
	glBindVertexArray(0);
}
