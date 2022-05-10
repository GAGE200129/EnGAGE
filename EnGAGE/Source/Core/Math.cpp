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
}
