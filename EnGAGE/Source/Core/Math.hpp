#pragma once

namespace Core::Math
{

	struct Frustum
	{
		Vec4 topFace;
		Vec4 bottomFace;
		Vec4 rightFace;
		Vec4 leftFace;
		Vec4 farFace;
		Vec4 nearFace;
	};

	Mat4x4 calculateProjectionView();
	Frustum createFrustum();
}