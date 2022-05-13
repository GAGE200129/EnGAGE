#pragma once

#include "Core/Shader.hpp"

namespace Core
{
	class AmbientShader : public Shader
	{
	public:
		AmbientShader();

		void uploadAmbient(const glm::vec3& v);
	private:
		int mAmbientLoc;
	};

	class AmbientRenderer
	{
	public:
		AmbientRenderer();
		~AmbientRenderer();

		void render();
	private:
		AmbientShader mShader;
		Vec3 mAmbientColor;
	};
}