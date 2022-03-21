project "lua"
	location "Libraries/lua"
	kind "StaticLib"
	language "C"
	targetdir "%{binary_location}"
	objdir "%{object_location}"

	files { 
		"%{prj.location}/*.c",
		"%{prj.location}/*.h",
	}
	removefiles { "%{prj.location}/onelua.c" }
	
	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"
		