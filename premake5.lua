workspace "EnGAGE"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	startproject "EnGAGE"

--Global vars
binary_location = "Binaries/%{prj.name}/%{cfg.buildcfg}"
object_location = "Objects/%{prj.name}/%{cfg.buildcfg}"


include "GLFW.lua"
include "spdlog.lua"
include "glad.lua"
include "EnGAGE.lua"

