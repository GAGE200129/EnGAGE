#pragma once

namespace Core
{
	struct Camera
	{
		enum class Mode
		{
			PERSPECTIVE,
			ORTHOGRAPHIC
		};
		Mode mode;
		F32 x, y, z;
		F32 pitch, yaw, roll;
		F32 fov, near, far;
		F32 minX, maxX, minY, maxY, minZ, maxZ;

	};
}