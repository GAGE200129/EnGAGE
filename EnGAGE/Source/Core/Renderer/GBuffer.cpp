#include "pch.hpp"
#include "GBuffer.hpp"

#include <glad/glad.h>

Core::GBuffer::GBuffer()
{
	constexpr float data[] =
	{
		 -1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f,  1.0f, 1.0f, 1.0f,
	};

	glGenFramebuffers(1, &FBO);
	//glGenRenderbuffers(1, &RBO);
	glGenTextures(1, &positionTex);
	glGenTextures(1, &normalTex);
	glGenTextures(1, &colorTex);
	glGenTextures(1, &depthTex);

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Core::GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1, &FBO);
	//glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &positionTex);
	glDeleteTextures(1, &normalTex);
	glDeleteTextures(1, &colorTex);
	glDeleteTextures(1, &depthTex);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}

void Core::GBuffer::update(UInt32 inWidth, UInt32 inHeight, F32 scale)
{
	UInt32 width = inWidth * scale;
	UInt32 height = inHeight * scale;

	auto minFilter = GL_NEAREST;
	auto magFilter = GL_NEAREST;
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, positionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTex, 0);

	glBindTexture(GL_TEXTURE_2D, normalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTex, 0);

	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

	unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	//glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	post.update(inWidth, inHeight, scale);
}

void Core::GBuffer::bind(UInt32 inWidth, UInt32 inHeight, F32 scale)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, inWidth * scale, inHeight * scale);
}

void Core::GBuffer::unBind(UInt32 inWidth, UInt32 inHeight)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, inWidth, inHeight);
}

void Core::GBuffer::bindQuad()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, positionTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(quadVAO);
}

void Core::GBuffer::renderQuad() const
{
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Core::GBuffer::unBindQuad()
{
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

Core::PostProcess::PostProcess()
{
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &colorTex);
}

Core::PostProcess::~PostProcess()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &colorTex);
}

void Core::PostProcess::update(UInt32 inWidth, UInt32 inHeight, F32 scale)
{
	UInt32 width = inWidth * scale;
	UInt32 height = inHeight * scale;

	auto minFilter = GL_NEAREST;
	auto magFilter = GL_NEAREST;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Core::PostProcess::bind(UInt32 inWidth, UInt32 inHeight, F32 scale) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, inWidth * scale, inHeight * scale);
}

void Core::PostProcess::unbind(UInt32 inWidth, UInt32 inHeight) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, inWidth, inHeight);
}
