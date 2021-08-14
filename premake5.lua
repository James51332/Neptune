workspace "Neptune"
	location "build"
	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	startproject "Sandbox"	
	staticruntime "On"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

project "Neptune"
	kind "StaticLib"
	location "build"

	targetdir ("build/bin/neptune-" .. string.lower("%{cfg.system}") .. "/%{cfg.shortname}/")
	objdir ("build/bin-obj/neptune-" .. string.lower("%{cfg.system}") .. "/%{cfg.shortname}/")

	files
	{
		"src/**.cpp",
		"src/**.h",
		"include/**.h"
	}

	includedirs
	{
		"src"
	}

	filter "system:macosx"
		defines "NEPTUNE_MACOS"

	filter "system:windows"
		defines "NEPTUNE_WINDOWS"
		systemversion "latest"

	filter "configurations:Debug"
		defines "NEPTUNE_DEBUG"
		symbols "On"		

	filter "configurations:Release"
		defines "NEPTUNE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "NEPTUNE_DIST"
		optimize "Full"

project "Sandbox"
	kind "ConsoleApp"
	
	debugdir "./"

	targetdir ("build/bin/neptune-" .. string.lower("%{cfg.system}") .. "/%{cfg.shortname}/")
        objdir ("build/bin-obj/neptune-" .. string.lower("%{cfg.system}") .. "/%{cfg.shortname}/")

	files
	{
		"sandbox/**.cpp",
		"sandbox/**.h"
	} 

	links
	{
		"Neptune"
	}
	
	includedirs
	{
		"src"
	}

	sysincludedirs
	{
		"include"
	}

	filter "system:macosx"
                defines "NEPTUNE_MACOS"

        filter "system:windows"
                defines "NEPTUNE_WINDOWS"
                systemversion "latest"

        filter "configurations:Debug"
                defines "NEPTUNE_DEBUG"
		symbols "On"

        filter "configurations:Release"
                defines "NEPTUNE_RELEASE"
		optimize "On"
	
        filter "configurations:Dist"
                defines "NEPTUNE_DIST"
		optimize "Full"
