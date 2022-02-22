project "EnGAGE"
	location "EnGAGE"
	kind "ConsoleApp"
	language "C++"
	targetdir "%{binary_location}"
	objdir "%{object_location}"


	includedirs {
		"Libraries/GLFW/include"
	}

	links {
		"GLFW"
	}


	files { "%{prj.location}/Source/**.hpp", "%{prj.location}/Source/**.cpp" }

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"


	filter "configurations:Debug"
		defines { "EN_DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "EN_NDEBUG" }
		optimize "On"