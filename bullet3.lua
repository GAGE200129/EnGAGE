project "bullet3"
	location "Libraries/bullet3"
	kind "StaticLib"
	language "C++"
	targetdir "%{binary_location}"
	objdir "%{object_location}"
	vectorextensions "SSE2"
	
	
	files { 
		"%{prj.location}/src/**.cpp",
	}
	excludes {
		"%{prj.location}/src/Bullet3OpenCL/**.cpp",
		"%{prj.location}/src/btBulletCollisionAll.cpp",
		"%{prj.location}/src/btLinearMathAll.cpp",
		"%{prj.location}/src/btBulletDynamicsAll.cpp"
	}
	includedirs { 
		"%{prj.location}/src"
	}
	
	defines {
		--"BT_USE_SSE_IN_API"
	}
	
	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Release"
		optimize "on"

	filter "system:Windows"
		systemversion "latest"
		staticruntime "On"
		disablewarnings { "4267", "4244", "4305", "4005" }
		