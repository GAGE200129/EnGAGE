workspace "EnGAGE"
	configurations { "Debug", "Release" }
	architecture "x86_64"

--Global vars
binary_location = "bin/%{prj.name}/%{cfg.buildcfg}"
object_location = "obj/%{prj.name}/%{cfg.buildcfg}"

project "EnGAGE"
	location "EnGAGE"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin/%{prj.name}/%{cfg.buildcfg}"
	objdir "obj/%{prj.name}/%{cfg.buildcfg}"

	files { "src/**.hpp", "src/**.cpp" }

	filter "configurations:Debug"
		defines { "EN_DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "EN_NDEBUG" }
		optimize "On"