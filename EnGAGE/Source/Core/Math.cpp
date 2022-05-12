#include "pch.hpp"
#include "Math.hpp"

#include "Renderer.hpp"
#include "Window.hpp"

namespace Core::Math
{
	Mat4x4 calculateProjectionView()
	{
		const auto& camera = Renderer::getCamera();
		const F32 aspect = (F32)Window::getWidth() / (F32)Window::getHeight();

		Mat4x4 proj = glm::perspective(glm::radians(camera.fov), aspect, camera.near, camera.far);

		Mat4x4 view = glm::rotate(Mat4x4(1.0f), -glm::radians(camera.pitch), { 1, 0, 0 });
		view = glm::rotate(view, -glm::radians(camera.yaw), { 0, 1, 0 });
		view = glm::translate(view, { -camera.x, -camera.y, -camera.z });
		return proj * view;
	}
	Frustum createFrustum()
	{
		Frustum     frustum = { {}, {}, {}, {}, {} };
		const auto& camera = Renderer::getCamera();

		//Get width and height of near and far plane
		const F32 aspect = (F32)Window::getWidth() / (F32)Window::getHeight();
		const float halfFov = glm::radians(camera.fov * 0.5f);
		const float Hnear = camera.near * glm::tan(halfFov) * 2.0f;
		const float Wnear = Hnear * aspect;

		const float Hfar = camera.far * glm::tan(halfFov) * 2.0f; 
		const float Wfar = Hfar * aspect;;

		const float farDist = camera.far;
		const float nearDist = camera.near;

		//Get forwad, up and right camera vector

		//Get rotation mat
		Mat4x4 rotation = glm::rotate(Mat4x4(1.0f), glm::radians(camera.yaw), { 0, 1, 0 });
		rotation = glm::rotate(rotation, glm::radians(camera.pitch), { 1, 0, 0 });

		//Rotate global unit vector
		Vec3 d = glm::normalize(Vec3(rotation * Vec4{ 0, 0, -1, 0 }));
		Vec3 right = glm::normalize(glm::cross(d, Vec3{ 0, 1, 0 }));
		Vec3 up = glm::normalize(glm::cross(right, d));
		Vec3 p = { camera.x, camera.y, camera.z };

		//Compute all frustum points
		
		Vec3 fc = p + d * camera.far;

		Vec3 ftl = fc + (up * Hfar / 2.0f) - (right * Wfar / 2.0f);
		Vec3 ftr = fc + (up * Hfar / 2.0f) + (right * Wfar / 2.0f);
		Vec3 fbl = fc - (up * Hfar / 2.0f) - (right * Wfar / 2.0f);
		Vec3 fbr = fc - (up * Hfar / 2.0f) + (right * Wfar / 2.0f);
		Vec3 nc = p + d * nearDist;
		Vec3 ntl = nc + (up * Hnear / 2.0f) - (right * Wnear / 2.0f);
		Vec3 ntr = nc + (up * Hnear / 2.0f) + (right * Wnear / 2.0f);
		Vec3 nbl = nc - (up * Hnear / 2.0f) - (right * Wnear / 2.0f);
		Vec3 nbr = nc - (up * Hnear / 2.0f) + (right * Wnear / 2.0f);

		//Build frustum planes

		auto buildPlaneFrom3Points = [](const Vec3& p1, const Vec3& p2, const Vec3& p3) -> Vec4
		{
			Vec3 p21 = p2 - p1;
			Vec3 p23 = p2 - p3;
			Vec3 normal = glm::normalize(glm::cross(p21, p23));
			float distance = -glm::dot(normal, p1);
			return {normal, distance};
		};

		frustum.nearFace = Vec4(d, -glm::dot(d, nc));
		frustum.farFace = Vec4(-d, -glm::dot(-d, fc));
		frustum.rightFace = buildPlaneFrom3Points(ftr, ntr, fbr);
		frustum.leftFace = buildPlaneFrom3Points(ntl, ftl, fbl);
		frustum.topFace = buildPlaneFrom3Points(ntr, ftr, ftl);
		frustum.bottomFace = buildPlaneFrom3Points(fbl, fbr, nbr);

		//EN_INFO("{} {} {} {} ", frustum.nearFace.x, frustum.nearFace.y, frustum.nearFace.z, frustum.nearFace.w);



		return frustum;
	}
}
