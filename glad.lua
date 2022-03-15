project "glad"
	location "Libraries/glad"
	kind "StaticLib"
	language "C"
	targetdir "%{binary_location}"
	objdir "%{object_location}"

	files { 
			"%{prj.location}/src/glad.c",
	}
	includedirs { "%{prj.location}/include"}
	
	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"
		