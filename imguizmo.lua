project "imguizmo"
	location "Libraries/ImGuizmo"
	kind "StaticLib"
	language "C++"
	targetdir "%{binary_location}"
	objdir "%{object_location}"

	files { 
		"%{prj.location}/*.cpp"
	}
	includedirs { 
		"%{prj.location}",
		"Libraries/imgui",
	}
	
	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"
		