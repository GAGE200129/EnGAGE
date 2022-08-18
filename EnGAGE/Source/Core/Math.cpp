#include "pch.hpp"
#include "Math.hpp"

#include "Window.hpp"
#include "Input.hpp"

namespace Core::Math
{
	Mat4x4 calculateProj(const Camera& camera)
	{
		const WindowData& windowData = Window::getData();
		const F32 aspect = (F32)windowData.screenWidth / (F32)windowData.screenHeight;
		Mat4x4 proj;
		if (camera.mode == Camera::Mode::PERSPECTIVE)
			proj = glm::perspective(glm::radians(camera.fov), aspect, camera.near, camera.far);
		else if (camera.mode == Camera::Mode::ORTHOGRAPHIC)
			proj = glm::ortho(camera.minX, camera.maxX, camera.minY, camera.maxY, camera.minZ, camera.maxZ);
		return proj;
	}
	Mat4x4 calculateView(const Camera& camera)
	{
		Mat4x4 view = glm::rotate(Mat4x4(1.0f), -glm::radians(camera.pitch), { 1, 0, 0 });
		view = glm::rotate(view, -glm::radians(camera.yaw), { 0, 1, 0 });
		view = glm::translate(view, { -camera.x, -camera.y, -camera.z });
		return view;
	}
	Mat4x4 calculateProjectionView(const Camera& camera)
	{
		return calculateProj(camera) * calculateView(camera);
	}
	Mat4x4 calDirectionalProjView(const Camera& camera, const Vec3& direction)
	{
		FrustumPoints points = createFrustumPoints(camera, camera.near, camera.far / 16.0f);
		Vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : points.points)
		{
			center += v;
		}
		center /= 8;

		const auto lightView = glm::lookAt(center + direction, center, Vec3(0.0f, 1.0f, 0.0f));
		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();
		for (const auto& v : points.points)
		{
			const auto trf = lightView * Vec4(v, 1);
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		constexpr float zMult = 10.0f;
		if (minZ < 0)
		{
			minZ *= zMult;
		}
		else
		{
			minZ /= zMult;
		}
		if (maxZ < 0)
		{
			maxZ /= zMult;
		}
		else
		{
			maxZ *= zMult;
		}
		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		return lightProjection * lightView;
	}
	Mat4x4 calCSMShadowMapProjView(const Vec3& lightDir, F32 zMult, const Camera& camera, F32 near, F32 far)
	{
		//Create shadow map view proj
		auto points = Math::createFrustumPoints(camera, near, far);

		Vec3 center = Vec3(0, 0, 0);
		for (const auto& v : points.points)
		{
			center += v;
		}
		center /= 8;

		Mat4x4 view = glm::lookAt(center - lightDir, center, Vec3{ 0.0f, 1.0f, 0.0f });
		F32 minX = std::numeric_limits<float>::max();
		F32 maxX = std::numeric_limits<float>::min();
		F32 minY = std::numeric_limits<float>::max();
		F32 maxY = std::numeric_limits<float>::min();
		F32 minZ = std::numeric_limits<float>::max();
		F32 maxZ = std::numeric_limits<float>::min();
		for (const auto& v : points.points)
		{
			const auto trf = view * Vec4(v, 1);
			minX = glm::min(minX, trf.x);
			maxX = glm::max(maxX, trf.x);
			minY = glm::min(minY, trf.y);
			maxY = glm::max(maxY, trf.y);
			minZ = glm::min(minZ, trf.z);
			maxZ = glm::max(maxZ, trf.z);
		}
		if (minZ < 0)
		{
			minZ *= zMult;
		}
		else
		{
			minZ /= zMult;
		}
		if (maxZ < 0)
		{
			maxZ /= zMult;
		}
		else
		{
			maxZ *= zMult;
		}
		return glm::ortho(minX, maxX, minY, maxY, minZ, maxZ) * view;
	}
	Frustum createFrustum(const Camera& camera)
	{
		Frustum frustum;
		
		//Build frustum planes

		auto buildPlaneFrom3Points = [](const Vec3& p1, const Vec3& p2, const Vec3& p3) -> Vec4
		{
			Vec3 p21 = p2 - p1;
			Vec3 p23 = p2 - p3;
			Vec3 normal = glm::normalize(glm::cross(p21, p23));
			float distance = -glm::dot(normal, p1);
			return {normal, distance};
		};
		FrustumPoints points = createFrustumPoints(camera, camera.near, camera.far);
		frustum.nearFace = Vec4(points.d, -glm::dot(points.d, points.nc));
		frustum.farFace = Vec4(-points.d, -glm::dot(-points.d, points.fc));
		frustum.rightFace = buildPlaneFrom3Points(points.ftr, points.ntr, points.fbr);
		frustum.leftFace = buildPlaneFrom3Points(points.ntl, points.ftl, points.fbl);
		frustum.topFace = buildPlaneFrom3Points(points.ntr, points.ftr, points.ftl);
		frustum.bottomFace = buildPlaneFrom3Points(points.fbl, points.fbr, points.nbr);
		return frustum;
	}
	FrustumPoints createFrustumPoints(const Camera& camera, const F32 near, const F32 far)
	{
		//Get width and height of near and far plane
		const WindowData& windowData = Window::getData();
		const F32 aspect = (F32)windowData.screenWidth / (F32)windowData.screenHeight;
		const float halfFov = glm::radians(camera.fov * 0.5f);
		const float Hnear = near * glm::tan(halfFov) * 2.0f;
		const float Wnear = Hnear * aspect;

		const float Hfar = far * glm::tan(halfFov) * 2.0f;
		const float Wfar = Hfar * aspect;;

		const float farDist = far;
		const float nearDist = near;

		//Get forwad, up and right camera vector

		//Get rotation mat
		Mat4x4 rotation = glm::rotate(Mat4x4(1.0f), glm::radians(camera.yaw), { 0, 1, 0 });
		rotation = glm::rotate(rotation, glm::radians(camera.pitch), { 1, 0, 0 });

		FrustumPoints points;
		//Rotate global unit vector
		points.d = glm::normalize(Vec3(rotation * Vec4{ 0, 0, -1, 0 }));
		points.right = glm::normalize(glm::cross(points.d, Vec3{ 0, 1, 0 }));
		points.up = glm::normalize(glm::cross(points.right, points.d));
		points.p = { camera.x, camera.y, camera.z };

		//Compute all frustum points
		points.fc = points.p + points.d * far;
		points.nc = points.p + points.d * nearDist;
		points.ftl = points.fc + (points.up * Hfar / 2.0f) - (points.right * Wfar / 2.0f);
		points.ftr = points.fc + (points.up * Hfar / 2.0f) + (points.right * Wfar / 2.0f);
		points.fbl = points.fc - (points.up * Hfar / 2.0f) - (points.right * Wfar / 2.0f);
		points.fbr = points.fc - (points.up * Hfar / 2.0f) + (points.right * Wfar / 2.0f);
		points.ntl = points.nc + (points.up * Hnear / 2.0f) - (points.right * Wnear / 2.0f);
		points.ntr = points.nc + (points.up * Hnear / 2.0f) + (points.right * Wnear / 2.0f);
		points.nbl = points.nc - (points.up * Hnear / 2.0f) - (points.right * Wnear / 2.0f);
		points.nbr = points.nc - (points.up * Hnear / 2.0f) + (points.right * Wnear / 2.0f);
		return points;
	}

	void getCamFrontRay(const Camera& camera, Vec3& outPosition, Vec3& outRay)
	{
		outPosition.x = camera.x;
		outPosition.y = camera.y;
		outPosition.z = camera.z;
		const WindowData& windowData = Window::getData();
		const InputData& inputData = Input::getData();
		auto x = ((inputData.cursorX / windowData.screenWidth) - 0.5f) * 2;
		auto y = -((inputData.cursorY / windowData.screenHeight) - 0.5f) * 2;
		auto projViewInverse = glm::inverse(Math::calculateProjectionView(camera));
		Vec4 rayStart = projViewInverse * Vec4(0, 0, 0, 1);
		rayStart *= 1.0f / rayStart.w;

		Vec4 rayEnd = projViewInverse * Vec4(0, 0, 1, 1);
		rayEnd *= 1.0f / rayEnd.w;

		outRay = glm::normalize(Vec3(rayEnd) - Vec3(rayStart));
	}

	void getCursorRay(const Camera& camera, Vec3& outPosition, Vec3& outRay)
	{
		outPosition.x = camera.x;
		outPosition.y = camera.y;
		outPosition.z = camera.z;
		const WindowData& windowData = Window::getData();
		const InputData& inputData = Input::getData();
		auto x = ((inputData.cursorX / windowData.screenWidth) - 0.5f) * 2;
		auto y = -((inputData.cursorY / windowData.screenHeight) - 0.5f) * 2;
		auto projViewInverse = glm::inverse(Math::calculateProjectionView(camera));
		Vec4 rayStart = projViewInverse * Vec4(x, y, 0, 1);
		rayStart *= 1.0f / rayStart.w;

		Vec4 rayEnd = projViewInverse * Vec4(x, y, 1, 1);
		rayEnd *= 1.0f / rayEnd.w;

		outRay = glm::normalize(Vec3(rayEnd) - Vec3(rayStart));
	}
	bool isRaySphereIntersect(const Vec3& rayPos, const Vec3& rayDir, const Vec3& center, const F32 radius)
	{
		F32 L = glm::length(center - rayPos);
		F32 dot = glm::dot(rayDir, center - rayPos);
		F32 distance = glm::sqrt(L * L - dot * dot);
		return distance < radius;
	}
	bool isRayTriangleIntersect(const Vec3& rayPos, const Vec3& rayDir, const Vec3& p1, const Vec3& p2, const Vec3& p3)
	{
		//https://stackoverflow.com/questions/28165548/ray-triangle-intersection-c
		glm::vec3 e1, e2, pvec, qvec, tvec;

		e1 = p2 - p1;
		e2 = p3 - p1;
		pvec = glm::cross(rayDir, e2);
		float det = glm::dot(pvec, e1);
		if (det != 0)
		{
			float invDet = 1.0f / det;
			tvec = rayPos - p1;
			float u = invDet * glm::dot(tvec, pvec);
			if (u < 0.0f || u > 1.0f)
			{

				return false;
			}
			qvec = glm::cross(tvec, e1);
			float v = invDet * glm::dot(qvec, rayDir);
			if (v < 0.0f || u + v > 1.0f)
			{

				return false;
			}
		}
		else 
			return false;
		return true;
	}
	btVector3 toBulletVec3(const glm::vec3& v)
	{
		return btVector3(v.x, v.y, v.z);
	}
}
