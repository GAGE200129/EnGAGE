project "GLFW"
	location "Libraries/GLFW"
	kind "StaticLib"
	language "C"
	targetdir "%{binary_location}"
	objdir "%{object_location}"

	--Global files
	files { 
			"%{prj.location}/include/GLFW/glfw3.h",
			"%{prj.location}/include/GLFW/glfw3native.h",
			"%{prj.location}/src/internal.h",
			"%{prj.location}/src/platform.h",
			"%{prj.location}/src/mappings.h",
			"%{prj.location}/src/context.c",
			"%{prj.location}/src/init.c",
			"%{prj.location}/src/input.c",
			"%{prj.location}/src/monitor.c",
			"%{prj.location}/src/platform.c",
			"%{prj.location}/src/vulkan.c",
			"%{prj.location}/src/window.c",
			"%{prj.location}/src/egl_context.c",
			"%{prj.location}/src/osmesa_context.c",
			"%{prj.location}/src/null_platform.h",
			"%{prj.location}/src/null_joystick.h",
			"%{prj.location}/src/null_init.c",
			"%{prj.location}/src/null_monitor.c",
			"%{prj.location}/src/null_window.c",
			"%{prj.location}/src/null_joystick.c",
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
	
	--Windows
	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			"%{prj.location}/src/win32_init.c",
			"%{prj.location}/src/win32_joystick.c",
			"%{prj.location}/src/win32_monitor.c",
			"%{prj.location}/src/win32_time.c",
			"%{prj.location}/src/win32_time.h",
			"%{prj.location}/src/win32_thread.c",
			"%{prj.location}/src/win32_thread.h",
			"%{prj.location}/src/win32_window.c",
			"%{prj.location}/src/win32_module.c",
			"%{prj.location}/src/wgl_context.c",
			"%{prj.location}/src/egl_context.c",
			"%{prj.location}/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		