#include "pch.hpp"
#include "DebugRenderer.hpp"
#include "Shader.hpp"
#include "Math.hpp"
#include "ECS.hpp"

#include <glad/glad.h>

namespace Core::DebugRenderer
{
	class BoxShader : public Shader
	{
	private:
		Int32 mModelViewProjLoc;
		Int32 mColorLoc;
		Int32 mMinLoc, mMaxLoc;
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
		void uploadBox(const Vec3& min, const Vec3& max)
		{
			uploadVec3(mMinLoc, min);
			uploadVec3(mMaxLoc, max);
		}

		void uploadColor(const Vec3& v)
		{
			uploadVec3(mColorLoc, v);
		}

		void uploadMVP(const Mat4x4& mat)
		{
			uploadMat4x4(mModelViewProjLoc, mat);
		}
	};
	class LineShader : public Shader
	{
	private:
		Int32 mModelViewProjLoc;
		Int32 mColorLoc;
		Int32 mBeginLoc, mEndLoc;
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

		void uploadLine(const Vec3& begin, const Vec3& end)
		{
			uploadVec3(mBeginLoc, begin);
			uploadVec3(mEndLoc, end);
		}

		void uploadColor(const Vec3& v)
		{
			uploadVec3(mColorLoc, v);
		}

		void uploadMVP(const Mat4x4& mat)
		{
			uploadMat4x4(mModelViewProjLoc, mat);
		}
	};

	struct Line
	{
		Vec3 color, begin, end;
	};
	struct Box
	{
		Vec3 color, min, max;
		Mat4x4 modelTransform;
	};
	static UInt32 gVAO, gVBO;
	static DynArr<Line> gLines;
	static DynArr<Box> gBoxes;
	static Scope<LineShader> gLineShader;
	static Scope<BoxShader> gBoxShader;

	void init()
	{
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

	void addLine(const Vec3& color, const Vec3& begin, const Vec3& end)
	{
		gLines.push_back({ color, begin, end });
	}

	void addBox(const Vec3& color, const Vec3& min, const Vec3& max, const Mat4x4& modelTransform)
	{
		gBoxes.push_back({ color, min, max , modelTransform });
	}

	void render()
	{
		Mat4x4 projView = Math::calculateProjectionView();
		glBindVertexArray(gVAO);
		gLineShader->bind();
		gLineShader->uploadMVP(projView);
		for (const auto& line : gLines)
		{
			gLineShader->uploadColor(line.color);
			gLineShader->uploadLine(line.begin, line.end);
			glDrawArrays(GL_POINTS, 0, 1);
		}

		gBoxShader->bind();
		for (const auto& box : gBoxes)
		{
			gBoxShader->uploadMVP(projView * box.modelTransform);
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
