project "EnGAGE"
	location "EnGAGE"
	kind "ConsoleApp"
	language "C++"
	targetdir "%{binary_location}"
	objdir "%{object_location}"

	pchsource "%{prj.location}/Source/pch.cpp"
	pchheader "pch.hpp"

	includedirs {
		"Libraries/GLFW/include",
		"Libraries/spdlog/include",
		"%{prj.location}/Source"
	}

	links {
		"GLFW",
		"spdlog"
	}

	defines {"SPDLOG_COMPILED_LIB"}


	files { "%{prj.location}/Source/**.hpp", "%{prj.location}/Source/**.cpp" }

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"

		defines {"EN_PLATFORM_WINDOWS"}


	filter "configurations:Debug"
		defines { "EN_DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "EN_NDEBUG" }
		optimize "On"