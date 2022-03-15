project "imgui"
	location "Libraries/imgui"
	kind "StaticLib"
	language "C++"
	targetdir "%{binary_location}"
	objdir "%{object_location}"

	files { 
		"%{prj.location}/*.cpp",
		"%{prj.location}/backends/imgui_impl_glfw.cpp",
		"%{prj.location}/backends/imgui_impl_opengl3.cpp",
	}
	includedirs { 
		"%{prj.location}",
		"Libraries/GLFW/include"
	}
	
	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"
		