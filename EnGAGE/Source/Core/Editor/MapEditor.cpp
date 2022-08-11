#include "pch.hpp"
#include "MapEditor.hpp"

#include "Core/Map/Map.hpp"

#include "Core/Math.hpp"
#include "Core/Input.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Renderer/DebugRenderer.hpp"
#include "Core/Resource.hpp"
#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

namespace Core::MapEditor
{
	static glm::vec3 cursorPos = { 0, 0, 0 };
	static glm::vec3 snap = { 0.1f, 0.1f, 0.1f };
	static bool selectMode = false;
	static bool vertexSelectMode = false;
	static long long selectedFace = -1;
	static unsigned short selectedVertex = 0;

	static constexpr glm::vec3 WALL_COLOR = { 1, 1, 0 };
	static constexpr glm::vec3 WALL_SELECT_COLOR = { 0, 1, 1 };
	static const char* FACE_NAMES[] = { "Front", "Back", "Left", "Right", "Top", "Bottom" };


	static void render3DCursor()
	{
		auto& camera = GameEngine::getEngineData().debugCamera;
		Mat4x4 proj = Math::calculateProj(camera);
		Mat4x4 view = Math::calculateView(camera);
		if (!selectMode && !vertexSelectMode && selectedFace == -1) 
		{
			Mat4x4 mat = glm::translate(Mat4x4(1.0f), cursorPos);
			if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD, glm::value_ptr(mat), 0, &snap.x))
			{
				F32 matrixTranslation[3], matrixRotation[3], matrixScale[3];
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(mat), matrixTranslation, matrixRotation, matrixScale);
				cursorPos.x = matrixTranslation[0];
				cursorPos.y = matrixTranslation[1];
				cursorPos.z = matrixTranslation[2];
			}
		}
	}

	static void processImGui()
	{
		auto& camera = GameEngine::getEngineData().debugCamera;
		ImGui::Begin("Map editor");
		if (ImGui::Button("Reset cursor"))
		{
			cursorPos = { 0, 0, 0 };
		}

		if (ImGui::Button("Set cursor to cam"))
		{
			cursorPos = Vec3I{ camera.x, camera.y, camera.z };
		}
		ImGui::Separator();
		if (ImGui::Button("New wall"))
		{
			Map::addFace(cursorPos, cursorPos + glm::vec3{ 1, 0, 0 }, cursorPos + glm::vec3{ 1, 0, -1 }, cursorPos + glm::vec3{ 0, 0, -1 }, {0, 0});
			selectedFace = Map::getData().faces.size() - 1;
		}
		if (ImGui::Button("Select wall"))
		{
			selectMode = true;
		}
		if (selectedFace != -1 && ImGui::Button("Select vertex"))
		{
			vertexSelectMode = true;
		}
		if (ImGui::Button("Deselect wall"))
		{
			selectMode = false;
			selectedFace = -1;
		}
		if (ImGui::Button("Delete wall") && selectedFace != -1)
		{
			Map::removeFace(selectedFace);
			selectedFace = -1;

		}

		ImGui::Separator();
		WallMesh& mesh = Map::getData().wallMesh;
		TextureSheet* texture = mesh.textureSheet;
		ImGui::Text("Texture slot: %p", texture);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_SHEET"))
			{
				TextureSheet* recievedTexture = Resource::getTextureSheet((const char*)payload->Data);
				mesh.textureSheet = recievedTexture;
			}
			ImGui::EndDragDropTarget();
		}

		if (texture)
		{
			ImGui::Image((ImTextureID)texture->id, ImVec2(texture->width, texture->height));
			if (ImGui::InputInt2("Texture div", &(texture->div.x)))
			{
				Map::updateVertexData();
			}

			F32 deltaX = 1.0f / texture->div.x;
			F32 deltaY = 1.0f / texture->div.y;
			Int32 id = 0;
			for (Int32 y = 0; y < texture->div.y; y++)
			{
				for (Int32 x = 0; x < texture->div.x; x++)
				{
					ImGui::PushID(id);
					if (ImGui::ImageButton((ImTextureID)texture->id,
						ImVec2(texture->width / texture->div.x, texture->height / texture->div.y),
						ImVec2(x * deltaX, y * deltaY),
						ImVec2(x * deltaX + deltaX, y * deltaY + deltaY))
						&& selectedFace != -1)
					{
						Map::getData().faces[selectedFace].texSlot = { x, y };
						Map::updateVertexData();
					}
					id++;
					ImGui::PopID();
					ImGui::SameLine();
				}
				ImGui::NewLine();
			}
		}



		ImGui::End();
	}

	static void processSelectFace()
	{
		if (selectedFace == -1) return;

		auto& faces = Map::getData().faces;
		auto& camera = GameEngine::getEngineData().debugCamera;
		auto& currentFace = faces[selectedFace];


		//Vec3 center = (*p1 + *p2 + *p3 + *p4) / 4.0f;

		//Mat4x4 proj = Math::calculateProj(camera);
		//Mat4x4 view = Math::calculateView(camera);
		//Mat4x4 mat = glm::translate(Mat4x4(1.0f), center);
		//Mat4x4 delta;
		//if (ImGuizmo::Manipulate(
		//	glm::value_ptr(view),
		//	glm::value_ptr(proj),
		//	ImGuizmo::OPERATION::TRANSLATE,
		//	ImGuizmo::MODE::WORLD,
		//	glm::value_ptr(mat), glm::value_ptr(delta), &snap.x))
		//{
		//	F32 matrixTranslation[3], matrixRotation[3], matrixScale[3];
		//	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(delta), matrixTranslation, matrixRotation, matrixScale);
		//	p1->x += matrixTranslation[0]; p2->x += matrixTranslation[0]; p3->x += matrixTranslation[0]; p4->x += matrixTranslation[0];
		//	p1->y += matrixTranslation[1]; p2->y += matrixTranslation[1]; p3->y += matrixTranslation[1]; p4->y += matrixTranslation[1];
		//	p1->z += matrixTranslation[2]; p2->z += matrixTranslation[2]; p3->z += matrixTranslation[2]; p4->z += matrixTranslation[2];
		//	Map::updateVertexData();
		//}
	}

	static void processSelect()
	{
		struct Intersect
		{
			std::vector<Face>::iterator it;
			Vec3 center;
		};

		auto renderOutline = [](const Vec3& color, const Face& face)
		{
			DebugRenderer::addLine(color, face.tri[0].p1, face.tri[0].p2);
			DebugRenderer::addLine(color, face.tri[0].p2, face.tri[0].p3);
			DebugRenderer::addLine(color, face.tri[0].p3, face.tri[0].p1);

			DebugRenderer::addLine(color, face.tri[1].p1, face.tri[1].p2);
			DebugRenderer::addLine(color, face.tri[1].p2, face.tri[1].p3);
			DebugRenderer::addLine(color, face.tri[1].p3, face.tri[1].p1);

		};


		//Render face out lines
		
		if (selectMode)
		{
			for (const auto& face : Map::getData().faces)
			{
				renderOutline({ 1, 1, 0 }, face);
			}
		}
		else if (selectedFace != -1)
		{
			renderOutline({ 1, 1, 0 }, Map::getData().faces[selectedFace]);
		}

		auto& faces = Map::getData().faces;
		auto& camera = GameEngine::getEngineData().debugCamera;
		if (selectMode && Input::isButtonDown(0))
		{
			//Get cursor ray
			Vec3 position, ray;
			Math::getCursorRay(camera, position, ray);

			DynArr<Intersect> intersects;
			for (auto it = faces.begin(); it != faces.end(); it++)
			{
				auto& face = *it;
				//Generate center point
				glm::vec3 center = (face.p1 + face.p2 + face.p3 + face.p4) / 4.0f;

				//Intersect ray triangle

				if (Math::isRayTriangleIntersect(position, ray, face.p1, face.p2, face.p3) ||
					Math::isRayTriangleIntersect(position, ray, face.p1, face.p3, face.p4))
				{
					intersects.push_back({ it, center});
				}
				
			}

			//Get the closest face
			selectedFace = -1;
			if (intersects.size() != 0)
			{
				Intersect closest = Intersect{ intersects.begin()->it, intersects.begin()->center };
				for (const auto& intersect : intersects)
				{
					F32 d1 = glm::length(closest.center - position);
					F32 d2 = glm::length(intersect.center - position);
					if (d2 < d1)
					{
						closest = Intersect{ intersect.it, intersect.center };
					}
				}
				selectedFace = closest.it - faces.begin();
			}

			selectMode = false;
		}
	}

	static void processVertexSelect()
	{
		if (selectedFace == -1) return;

		auto& face = Map::getData().faces[selectedFace];
		if (vertexSelectMode)
		{
			DebugRenderer::addSphere({ 1, 0, 0 }, 0.1f, face.p1);
			DebugRenderer::addSphere({ 1, 0, 0 }, 0.1f, face.p2);
			DebugRenderer::addSphere({ 1, 0, 0 }, 0.1f, face.p3);
			DebugRenderer::addSphere({ 1, 0, 0 }, 0.1f, face.p4);
		}

		auto& camera = GameEngine::getEngineData().debugCamera;
		if (vertexSelectMode && Input::isButtonDown(0))
		{
			//Get cursor ray
			glm::vec3 position, ray;
			Math::getCursorRay(camera, position, ray);

			selectedVertex = 0;
			for (unsigned int i = 0; i < 4; i++)
			{
				const glm::vec3& p = face.p[i];

				if (Math::isRaySphereIntersect(position, ray, p, 0.1f))
				{
					selectedVertex = i;
				}
			}
			vertexSelectMode = false;
		}


		if (selectedVertex != -1)
		{
			glm::vec3& vertex = face.p[selectedVertex];
			Mat4x4 proj = Math::calculateProj(camera);
			Mat4x4 view = Math::calculateView(camera);
			Mat4x4 mat = glm::translate(Mat4x4(1.0f), vertex);
			Mat4x4 delta;
			if (ImGuizmo::Manipulate(
				glm::value_ptr(view),
				glm::value_ptr(proj),
				ImGuizmo::OPERATION::TRANSLATE,
				ImGuizmo::MODE::WORLD,
				glm::value_ptr(mat), glm::value_ptr(delta), &snap.x))
			{
				F32 matrixTranslation[3], matrixRotation[3], matrixScale[3];
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(delta), matrixTranslation, matrixRotation, matrixScale);
				vertex.x += matrixTranslation[0];
				vertex.y += matrixTranslation[1];
				vertex.z += matrixTranslation[2];
				Map::updateVertexData();
			}
		}
	}


	void process()
	{
		render3DCursor();
		processImGui();
		processSelect();
		processSelectFace();
		processVertexSelect();
	}
	void clear()
	{
		cursorPos = { 0, 0, 0 };
		snap = { 0.1f, 0.1f, 0.1f };
		selectedFace = -1;
		selectMode = false;
	}
}
