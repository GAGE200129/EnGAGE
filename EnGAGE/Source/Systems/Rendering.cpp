#include "pch.hpp"
#include "Rendering.hpp"

#include <glad/glad.h>

Core::Rendering::Rendering()
{
	mShader = createScope<Core::Shader>();
	mShader->loadVertexShader("Resources/Shaders/test_VS.glsl");
	mShader->loadFragmentShader("Resources/Shaders/test_FS.glsl");
	mShader->compile();
}

void Core::Rendering::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	mShader->bind();
	for (const auto& component : mComponents)
	{
		glBindVertexArray(component->mVAO);
		glDrawElements(GL_TRIANGLES, component->mVertexCount, GL_UNSIGNED_INT, nullptr);
	}
}

void Core::Rendering::cleanup()
{
	mShader->cleanup();
}
