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
	static Vec3 cursorPos = { 0, 0, 0 };
	static Vec3 snap = { 0.1f, 0.1f, 0.1f };
	static Int64 selectedWall = -1;
	static bool selectMode = false;
	static UInt8 selectedFace = 0;

	static constexpr Vec3 WALL_COLOR = { 1, 1, 0 };
	static constexpr Vec3 WALL_SELECT_COLOR = { 0, 1, 1 };
	static const char* FACE_NAMES[] = { "Front", "Back", "Left", "Right", "Top", "Bottom" };

	static void renderOutlines()
	{

		auto renderOutline = [](const Vec3& color, const Wall& wall)
		{
			DebugRenderer::addLine(color, wall.p1, wall.p2);
			DebugRenderer::addLine(color, wall.p2, wall.p3);
			DebugRenderer::addLine(color, wall.p3, wall.p4);
			DebugRenderer::addLine(color, wall.p1, wall.p4);

			DebugRenderer::addLine(color, wall.p5, wall.p6);
			DebugRenderer::addLine(color, wall.p6, wall.p7);
			DebugRenderer::addLine(color, wall.p7, wall.p8);
			DebugRenderer::addLine(color, wall.p5, wall.p8);

			DebugRenderer::addLine(color, wall.p1, wall.p5);
			DebugRenderer::addLine(color, wall.p2, wall.p6);
			DebugRenderer::addLine(color, wall.p3, wall.p7);
			DebugRenderer::addLine(color, wall.p4, wall.p8);
		};

		if (selectMode)
		{
			auto& walls = Map::getWalls();
			for (UInt64 i = 0; i < walls.size(); i++)
			{
				const auto& wall = Map::getWalls()[i];
				renderOutline(WALL_COLOR, wall);
			}
		}
		else if (selectedWall != -1)
		{
			const auto& wall = Map::getWalls()[selectedWall];
			renderOutline(WALL_SELECT_COLOR, wall);
		}

	}

	static void render3DCursor()
	{
		auto& camera = GameEngine::getDebugCamera();
		Mat4x4 proj = Math::calculateProj(camera);
		Mat4x4 view = Math::calculateView(camera);
		if (!selectMode && selectedWall == -1)
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
		auto& camera = GameEngine::getDebugCamera();
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
			Map::addWall(Wall{
				cursorPos + Vec3{0, 0, 0},
				cursorPos + Vec3{0, 0, 1},
				cursorPos + Vec3{1, 0, 1},
				cursorPos + Vec3{1, 0, 0},
				cursorPos + Vec3{0, 1, 0},
				cursorPos + Vec3{0, 1, 1},
				cursorPos + Vec3{1, 1, 1},
				cursorPos + Vec3{1, 1, 0}
				});

			selectedWall = Map::getWalls().size() - 1;
		}
		if (ImGui::Button("Select wall"))
		{
			selectMode = true;
		}
		if (ImGui::Button("Deselect wall"))
		{
			selectMode = false;
			selectedWall = -1;
		}
		if (ImGui::Button("Delete wall") && selectedWall != -1)
		{
			Map::removeWall(selectedWall);
			selectedWall = -1;
		}
		if (selectedWall != -1 && ImGui::Button("SplitZY"))
		{
			Wall wall = Map::getWalls()[selectedWall];
			Vec3 half14 = (wall.p1 + wall.p4) / 2.0f;
			Vec3 half23 = (wall.p2 + wall.p3) / 2.0f;
			Vec3 half67 = (wall.p6 + wall.p7) / 2.0f;
			Vec3 half58 = (wall.p5 + wall.p8) / 2.0f;

			Wall newWall = { wall.p1, wall.p2, half23, half14, wall.p5, wall.p6, half67, half58,
							wall.front, wall.back, wall.left, wall.right, wall.top, wall.bottom };
			Wall newWall2 = { half14, half23, wall.p3, wall.p4, half58, half67, wall.p7, wall.p8,
						wall.front, wall.back, wall.left, wall.right, wall.top, wall.bottom };
			Map::removeWall(selectedWall);
			Map::addWall(newWall);
			Map::addWall(newWall2);
			selectedWall = -1;
		}
		if (selectedWall != -1 && ImGui::Button("SplitZX"))
		{
			Wall wall = Map::getWalls()[selectedWall];
			Vec3 half15 = (wall.p1 + wall.p5) / 2.0f;
			Vec3 half26 = (wall.p2 + wall.p6) / 2.0f;
			Vec3 half37 = (wall.p3 + wall.p7) / 2.0f;
			Vec3 half48 = (wall.p4 + wall.p8) / 2.0f;

			Wall newWall = { wall.p1, wall.p2, wall.p3, wall.p4, half15, half26, half37, half48,
							wall.front, wall.back, wall.left, wall.right, wall.top, wall.bottom };
			Wall newWall2 = { half15, half26, half37, half48, wall.p5, wall.p6, wall.p7, wall.p8,
						wall.front, wall.back, wall.left, wall.right, wall.top, wall.bottom };
			Map::removeWall(selectedWall);
			Map::addWall(newWall);
			Map::addWall(newWall2);
			selectedWall = -1;
		}
		if (selectedWall != -1 && ImGui::Button("SplitXY"))
		{
			Wall wall = Map::getWalls()[selectedWall];
			Vec3 half12 = (wall.p1 + wall.p2) / 2.0f;
			Vec3 half34 = (wall.p3 + wall.p4) / 2.0f;
			Vec3 half56 = (wall.p5 + wall.p6) / 2.0f;
			Vec3 half78 = (wall.p7 + wall.p8) / 2.0f;

			Wall newWall = { wall.p1, half12, half34, wall.p4, wall.p5, half56, half78, wall.p8,
							wall.front, wall.back, wall.left, wall.right, wall.top, wall.bottom };
			Wall newWall2 = { half12, wall.p2, wall.p3, half34, half56, wall.p6, wall.p7, half78,
						wall.front, wall.back, wall.left, wall.right, wall.top, wall.bottom };
			Map::removeWall(selectedWall);
			Map::addWall(newWall);
			Map::addWall(newWall2);
			selectedWall = -1;
		}
		if (selectedWall != -1)
		{
			
			auto& wall = Map::getWalls()[selectedWall];
			auto& wallMesh = Map::getWallMesh();
			for (int n = 0; n < 6; n++)
			{
				if (ImGui::RadioButton(FACE_NAMES[n], selectedFace == n))
					selectedFace = n;
			}
		}
		ImGui::Separator();
		WallMesh& mesh = Map::getWallMesh();
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
						&& selectedWall != -1)
					{
						Map::getWalls()[selectedWall].slotCoords[selectedFace] = { x, y };
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
		auto& walls = Map::getWalls();
		auto& camera = GameEngine::getDebugCamera();
		auto& currentWall = walls[selectedWall];
		auto* wp1 = &currentWall.p1;
		auto* wp2 = &currentWall.p2;
		auto* wp3 = &currentWall.p3;
		auto* wp4 = &currentWall.p4;
		auto* wp5 = &currentWall.p5;
		auto* wp6 = &currentWall.p6;
		auto* wp7 = &currentWall.p7;
		auto* wp8 = &currentWall.p8;

		Vec3* p1 = wp1;
		Vec3* p2 = wp5;
		Vec3* p3 = wp8;
		Vec3* p4 = wp4;
		ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE_Z;

		if (selectedFace == 0) //Front
		{
			p1 = wp1;
			p2 = wp5;
			p3 = wp8;
			p4 = wp4;
			operation = ImGuizmo::OPERATION::TRANSLATE_Z;
		}
		else if (selectedFace == 1) //Back
		{
			p1 = wp2;
			p2 = wp3;
			p3 = wp7;
			p4 = wp6;
			operation = ImGuizmo::OPERATION::TRANSLATE_Z;
		}
		else if (selectedFace == 2) //Left
		{
			p1 = wp1;
			p2 = wp2;
			p3 = wp6;
			p4 = wp5;
			operation = ImGuizmo::OPERATION::TRANSLATE_X;
		}

		else if (selectedFace == 3) //Right
		{
			p1 = wp8;
			p2 = wp7;
			p3 = wp3;
			p4 = wp4;
			operation = ImGuizmo::OPERATION::TRANSLATE_X;
		}
		else if (selectedFace == 4) //Top
		{
			p1 = wp5;
			p2 = wp6;
			p3 = wp7;
			p4 = wp8;
			operation = ImGuizmo::OPERATION::TRANSLATE_Y;
		}
		else if (selectedFace == 5) //Bottom
		{
			p1 = wp1;
			p2 = wp2;
			p3 = wp3;
			p4 = wp4;
			operation = ImGuizmo::OPERATION::TRANSLATE_Y;
		}


		Vec3 center = (*p1 + *p2 + *p3 + *p4) / 4.0f;

		Mat4x4 proj = Math::calculateProj(camera);
		Mat4x4 view = Math::calculateView(camera);
		Mat4x4 mat = glm::translate(Mat4x4(1.0f), center);
		Mat4x4 delta;
		if (ImGuizmo::Manipulate(
			glm::value_ptr(view),
			glm::value_ptr(proj),
			operation, ImGuizmo::MODE::WORLD,
			glm::value_ptr(mat), glm::value_ptr(delta), &snap.x))
		{
			F32 matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(delta), matrixTranslation, matrixRotation, matrixScale);
			p1->x += matrixTranslation[0]; p2->x += matrixTranslation[0]; p3->x += matrixTranslation[0]; p4->x += matrixTranslation[0];
			p1->y += matrixTranslation[1]; p2->y += matrixTranslation[1]; p3->y += matrixTranslation[1]; p4->y += matrixTranslation[1];
			p1->z += matrixTranslation[2]; p2->z += matrixTranslation[2]; p3->z += matrixTranslation[2]; p4->z += matrixTranslation[2];
			Map::updateVertexData();
		}
	}

	static void processSelect()
	{
		struct Intersect
		{
			DynArr<Wall>::iterator it;
			Vec3 center;
		};

		//Render wall boxes
		auto& walls = Map::getWalls();
		auto& camera = GameEngine::getDebugCamera();
		if (selectMode && Input::isButtonDown(0))
		{
			//Get cursor ray
			Vec3 position, ray;
			Math::getCursorRay(camera, position, ray);

			DynArr<Intersect> intersects;
			for (auto it = walls.begin(); it != walls.end(); it++)
			{
				auto& wall = *it;
				//Generate bounding sphere
				float d17 = glm::length(wall.p1 - wall.p7);
				float d46 = glm::length(wall.p4 - wall.p6);
				float d28 = glm::length(wall.p2 - wall.p8);
				float d35 = glm::length(wall.p3 - wall.p5);
				float radius = std::max({ d17, d46, d28, d35 }) / 2.0f;
				Vec3 center = (wall.p1 + wall.p2 + wall.p3 + wall.p4 + wall.p5 + wall.p6 + wall.p7 + wall.p8) / 8.0f;

				//Intersect plane sphere
				if (Math::isRaySphereIntersect(position, ray, center, radius))
				{
					intersects.push_back({ it, center });
				}
			}

			selectedWall = -1;
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
				selectedWall = closest.it - walls.begin();
			}

			selectMode = false;
		}
	}


	void process()
	{
		renderOutlines();
		render3DCursor();
		processImGui();
		processSelect();
		if (selectedWall != -1)
			processSelectFace();
	}
	void clear()
	{
		cursorPos = { 0, 0, 0 };
		snap = { 0.1f, 0.1f, 0.1f };
		selectedWall = -1;
		selectMode = false;
		selectedFace = 0;
	}
}
