#pragma once

#include "Camera.hpp"

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
	Mat4x4 calculateProj(const Camera& camera);
	Mat4x4 calculateView(const Camera& camera);
	Mat4x4 calculateProjectionView(const Camera& camera);
	Mat4x4 calDirectionalProjView(const Camera& camera, const Vec3& direction);
	FrustumPoints createFrustumPoints(const Camera& camera, const F32 near, const F32 far);
	Frustum createFrustum(const Camera& camera);
	void getCamFrontRay(const Camera& camera, Vec3& outPosition, Vec3& outRay);
	void getCursorRay(const Camera& camera, Vec3& outPosition, Vec3& outRay);
	bool isRaySphereIntersect(const Vec3& rayPos, const Vec3& rayDir, const Vec3& center, const F32 radius);
}