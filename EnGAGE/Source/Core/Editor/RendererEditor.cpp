#include "pch.hpp"
#include "RendererEditor.hpp"

#include "Core/Messenger/Messenger.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Renderer/DebugRenderer.hpp"
#include "Core/Renderer/Renderer.hpp"
#include "Core/Math.hpp"

namespace Core::RendererEditor
{
	static void renderCameraFrustum()
	{
		static constexpr Vec3 frustumColor = { 1, 1, 1 };
		const auto& camera = GameEngine::getMainCamera();

		auto frustum = Math::createFrustumPoints(camera, camera.near, camera.far);

		DebugRenderer::addLine(frustumColor, frustum.nbl, frustum.nbr);
		DebugRenderer::addLine(frustumColor, frustum.ntl, frustum.ntr);
		DebugRenderer::addLine(frustumColor, frustum.ntl, frustum.nbl);
		DebugRenderer::addLine(frustumColor, frustum.ntr, frustum.nbr);

		DebugRenderer::addLine(frustumColor, frustum.fbl, frustum.fbr);
		DebugRenderer::addLine(frustumColor, frustum.ftl, frustum.ftr);
		DebugRenderer::addLine(frustumColor, frustum.ftl, frustum.fbl);
		DebugRenderer::addLine(frustumColor, frustum.ftr, frustum.fbr);

		DebugRenderer::addLine(frustumColor, frustum.ntl, frustum.ftl);
		DebugRenderer::addLine(frustumColor, frustum.ntr, frustum.ftr);
		DebugRenderer::addLine(frustumColor, frustum.nbl, frustum.fbl);
		DebugRenderer::addLine(frustumColor, frustum.nbr, frustum.fbr);
	}

	void process()
	{
		static bool drawAABB = false;
		static float renderScale = 1.0f;
		static bool drawCameraFrustum = false;

		ImGui::Begin("Renderer");
		if (ImGui::RadioButton("Toggle culling sphere", Renderer::isRenderCullingSphere()))
		{
			auto& boolean = Renderer::isRenderCullingSphere();
			boolean = !boolean;
		}
		ImGui::Separator();
		if (ImGui::RadioButton("Render camera frustum", drawCameraFrustum))
		{
			drawCameraFrustum = !drawCameraFrustum;
		}
		ImGui::Separator();
		if (ImGui::SliderFloat("Render Scale", &renderScale, 0.0f, 2.0f))
		{
			Renderer::setRenderScale(renderScale);
		}
		
		ImGui::End();

		if (drawCameraFrustum)
		{
			renderCameraFrustum();
		}
	}
}
