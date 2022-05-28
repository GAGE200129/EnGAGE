#include "pch.hpp"
#include "Console.hpp"

namespace Core::Console
{
	void process()
	{
		ImGui::Begin("Console");

		//Console output
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		const auto& stringLines = Core::Log::getStringLines();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

		for (const auto& line : stringLines) {
			ImVec4 color;
			bool has_color = false;

			if (has_color = (line.find("[info]") != std::string::npos))
			{
				color = ImVec4(0, 1, 0, 1);
			}
			else if (has_color = (line.find("[warn]") != std::string::npos))
			{
				color = ImVec4(1, 1, 0, 1);
			}

			else if (has_color = (line.find("[error]") != std::string::npos))
			{
				color = ImVec4(1, 0, 0, 1);
			}

			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(line.c_str());
			if (has_color)
				ImGui::PopStyleColor();
		}

		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::End();
	}
}
