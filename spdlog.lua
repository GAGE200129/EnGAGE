project "spdlog"
	location "Libraries/spdlog"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	targetdir "%{binary_location}"
	objdir "%{object_location}"

	includedirs {
		"%{prj.location}/include"
	}

	files {
		"%{prj.location}/include/**.h",
		"%{prj.location}/src/**.cpp"
	}

	defines {"SPDLOG_COMPILED_LIB"}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"