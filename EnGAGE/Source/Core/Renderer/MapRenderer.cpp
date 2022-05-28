#include "pch.hpp"
#include "MapRenderer.hpp"

#include "Core/Map/Map.hpp"
#include "Core/Math.hpp"

#include <glad/glad.h>

Core::MapRenderer::~MapRenderer()
{
	mShader.cleanup();
}

void Core::MapRenderer::render(GBuffer& gBuffer, const Camera& camera)
{
	auto& walls = Map::getWalls();
	if (walls.size() == 0) return;

	mShader.bind();
	mShader.uploadProjView(Math::calculateProjectionView(camera));
	glBindVertexArray(Map::getWallMesh());
	glDrawElements(GL_TRIANGLES, walls.size() * 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	
}
