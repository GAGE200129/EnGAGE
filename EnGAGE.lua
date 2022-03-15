project "EnGAGE"
	location "EnGAGE"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++14"
	targetdir "%{binary_location}"
	objdir "%{object_location}"

	pchsource "%{prj.location}/Source/pch.cpp"
	pchheader "pch.hpp"

	includedirs {
		"Libraries/GLFW/include",
		"Libraries/spdlog/include",
		"Libraries/glad/include",
		"Libraries/tinygltf",
		"Libraries/glm",
		"Libraries/imgui",
		"Libraries/imgui/backends",
		"%{prj.location}/Source"
	}

	links {
		"GLFW",
		"spdlog",
		"glad",
		"imgui"
	}

	defines {"SPDLOG_COMPILED_LIB", "_CRT_SECURE_NO_WARNINGS"}


	files { "%{prj.location}/Source/**.hpp", "%{prj.location}/Source/**.cpp" }

	filter "configurations:Debug"
		defines { "EN_DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "EN_NDEBUG" }
		optimize "On"

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"

		defines {"EN_PLATFORM_WINDOWS"}


