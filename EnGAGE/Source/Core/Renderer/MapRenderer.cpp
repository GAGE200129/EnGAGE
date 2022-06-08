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
	if (Map::getWalls().size() == 0) return;


	mShader.bind();
	mShader.uploadProjView(Math::calculateProjectionView(camera));

	auto& mesh = Map::getWallMesh();

	if (mesh.textureSheet != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.textureSheet->id);
	}
	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Core::MapRenderer::renderNoShader()
{
	if (Map::getWalls().size() == 0) return;
	auto& mesh = Map::getWallMesh();
	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.vertexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
