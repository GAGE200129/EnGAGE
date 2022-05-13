#pragma once

namespace Core::Math
{
	union FrustumPoints
	{
		struct 
		{
			Vec3 ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr, d, right, up, p, fc, nc;
		};

		Vec3 points[8];
	};
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
	Mat4x4 calDirectionalProjView(const Vec3& direction);
	FrustumPoints createFrustumPoints(const F32 near, const F32 far);
	Frustum createFrustum();
}