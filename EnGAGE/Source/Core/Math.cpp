#include "pch.hpp"
#include "Math.hpp"

#include "Window.hpp"

namespace Core::Math
{
	Mat4x4 calculateProjectionView(const Camera& camera)
	{
		const F32 aspect = (F32)Window::getWidth() / (F32)Window::getHeight();
		Mat4x4 proj;
		if (camera.mode == Camera::Mode::PERSPECTIVE)
			proj = glm::perspective(glm::radians(camera.fov), aspect, camera.near, camera.far);
		else if (camera.mode == Camera::Mode::ORTHOGRAPHIC)
			proj = glm::ortho(camera.minX, camera.maxX, camera.minY, camera.maxY, camera.minZ, camera.maxZ);

		Mat4x4 view = glm::rotate(Mat4x4(1.0f), -glm::radians(camera.pitch), { 1, 0, 0 });
		view = glm::rotate(view, -glm::radians(camera.yaw), { 0, 1, 0 });
		view = glm::translate(view, { -camera.x, -camera.y, -camera.z });
		return proj * view;
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
		const F32 aspect = (F32)Window::getWidth() / (F32)Window::getHeight();
		const float halfFov = glm::radians(camera.fov * 0.5f);
		const float Hnear = near * glm::tan(halfFov) * 2.0f;
		const float Wnear = Hnear * aspect;

		const float Hfar = far * glm::tan(halfFov) * 2.0f;
		const float Wfar = Hfar * aspect;;

		const float farDist = camera.far;
		const float nearDist = camera.near;

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
		points.fc = points.p + points.d * camera.far;
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
}
