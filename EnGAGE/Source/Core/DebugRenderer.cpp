#include "pch.hpp"
#include "DebugRenderer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "ECS.hpp"

#include <glad/glad.h>

namespace Core::DebugRenderer
{
	class BoxShader : public Shader
	{
	private:
		int mModelViewProjLoc;
		int mColorLoc;
		int mMinLoc, mMaxLoc;
	public:
		BoxShader()
		{
			loadVertexShader("Resources/Shaders/Debug_VS.glsl");
			loadGeometryShader("Resources/Shaders/Box_GS.glsl");
			loadFragmentShader("Resources/Shaders/Debug_FS.glsl");
			compile();

			mModelViewProjLoc = registerUniform("uMVP");
			mColorLoc = registerUniform("uColor");
			mMinLoc = registerUniform("uMin");
			mMaxLoc = registerUniform("uMax");
		}
		void uploadBox(const glm::vec3& min, const glm::vec3& max)
		{
			uploadVec3(mMinLoc, min);
			uploadVec3(mMaxLoc, max);
		}

		void uploadColor(const glm::vec3& v)
		{
			uploadVec3(mColorLoc, v);
		}

		void uploadMVP(const glm::mat4x4& mat)
		{
			uploadMat4x4(mModelViewProjLoc, mat);
		}
	};
	class LineShader : public Shader
	{
	private:
		int mModelViewProjLoc;
		int mColorLoc;
		int mBeginLoc, mEndLoc;
	public:
		LineShader()
		{
			loadVertexShader("Resources/Shaders/Debug_VS.glsl");
			loadGeometryShader("Resources/Shaders/Line_GS.glsl");
			loadFragmentShader("Resources/Shaders/Debug_FS.glsl");
			compile();

			mModelViewProjLoc = registerUniform("uMVP");
			mColorLoc = registerUniform("uColor");
			mBeginLoc = registerUniform("uBegin");
			mEndLoc = registerUniform("uEnd");
		}

		void uploadLine(const glm::vec3& begin, const glm::vec3& end)
		{
			uploadVec3(mBeginLoc, begin);
			uploadVec3(mEndLoc, end);
		}

		void uploadColor(const glm::vec3& v)
		{
			uploadVec3(mColorLoc, v);
		}

		void uploadMVP(const glm::mat4x4& mat)
		{
			uploadMat4x4(mModelViewProjLoc, mat);
		}
	};

	struct Line
	{
		glm::vec3 color, begin, end;
	};
	struct Box
	{
		glm::vec3 color, min, max;
		glm::mat4x4 modelTransform;
	};
	static unsigned int gScreenWidth, gScreenHeight;
	static unsigned int gVAO, gVBO;
	static DynArr<Line> gLines;
	static DynArr<Box> gBoxes;
	static Scope<LineShader> gLineShader;
	static Scope<BoxShader> gBoxShader;

	void init(unsigned int width, unsigned int height)
	{
		gScreenWidth = width;
		gScreenHeight = height;

		float data[] = { 0, 0, 0 };

		glGenVertexArrays(1, &gVAO);
		glGenBuffers(1, &gVBO);

		glBindVertexArray(gVAO);
		glBindBuffer(GL_VERTEX_ARRAY, gVBO);
		glBufferData(GL_VERTEX_ARRAY, sizeof(float) * 3, data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_VERTEX_ARRAY, 0);
		glBindVertexArray(0);

		gLineShader = createScope<LineShader>();
		gBoxShader = createScope<BoxShader>();
	}
	void onMessage(const Message* pMessage)
	{
		if (auto windowResized = Messenger::messageCast<MessageType::WINDOW_RESIZED, WindowResizedMessage>(pMessage))
		{
			gScreenWidth = windowResized->width;
			gScreenHeight = windowResized->height;
		}
	}
	void addLine(const glm::vec3& color, const glm::vec3& begin, const glm::vec3& end)
	{
		gLines.push_back({ color, begin, end });
	}

	void addBox(const glm::vec3& color, const glm::vec3& min, const glm::vec3& max, const glm::mat4x4& modelTransform)
	{
		gBoxes.push_back({ color, min, max , modelTransform });
	}

	void render()
	{
		if (gScreenHeight == 0) return;
		const auto& camera = Renderer::getCamera();
		const float aspect = (float)gScreenWidth / (float)gScreenHeight;

		glm::mat4x4 proj = glm::perspective(glm::radians(camera.fov), aspect, camera.near, camera.far);

		glm::mat4x4 view = glm::rotate(glm::mat4(1.0f), -glm::radians(camera.pitch), { 1, 0, 0 });
		view = glm::rotate(view, -glm::radians(camera.yaw), { 0, 1, 0 });
		view = glm::translate(view, { -camera.x, -camera.y, -camera.z });


		glBindVertexArray(gVAO);
		gLineShader->bind();
		gLineShader->uploadMVP(proj * view);	
		for (const auto& line : gLines)
		{
			gLineShader->uploadColor(line.color);
			gLineShader->uploadLine(line.begin, line.end);
			glDrawArrays(GL_POINTS, 0, 1);
		}

		gBoxShader->bind();
		for (const auto& box : gBoxes)
		{
			gBoxShader->uploadMVP(proj * view * box.modelTransform);
			gBoxShader->uploadColor(box.color);
			gBoxShader->uploadBox(box.min, box.max);
			glDrawArrays(GL_POINTS, 0, 1);
		}
		glBindVertexArray(0);

		gLines.clear();
		gBoxes.clear();
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &gVAO);
		glDeleteBuffers(1, &gVBO);
		gLineShader->cleanup();
		gBoxShader->cleanup();
	}

}
