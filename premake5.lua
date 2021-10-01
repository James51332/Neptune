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
	
includeDirs = {}
includeDirs["spdlog"] = "thirdparty/spdlog/include"
includeDirs["concurrentQueue"] = "thirdparty/concurrentQueue"

project "Neptune"
	kind "StaticLib"
	location "build"

	targetdir ("build/bin/neptune-" .. string.lower("%{cfg.system}") .. "/%{cfg.shortname}/")
	objdir ("build/bin-obj/neptune-" .. string.lower("%{cfg.system}") .. "/%{cfg.shortname}/")

	files
	{
		"src/**.cpp",
		"src/**.h",
		"include/**.h",
  	"pch/neptunepch.h",
   	"pch/neptunepch.cpp"
	}

	includedirs
	{
		"src",
  	"pch",
	}
	
	sysincludedirs
	{
		includeDirs["spdlog"],
  	includeDirs["concurrentQueue"]
	}

	filter "system:windows"
		systemversion "latest"
  
  filter "action:xcode4"
    pchheader "../pch/neptunepch.h"
    
  filter "action:vs*"
    pchheader "neptunepch.h"
    pchheader "neptunepch.cpp"

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
	
	debugdir (path.getabsolute("."))

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
		"src",
  	"pch"
	}

	sysincludedirs
	{
		"include",
  	includeDirs["spdlog"],
   	includeDirs["concurrentQueue"] 
	}
	
  filter "system:windows"
    systemversion "latest"
    
  filter "action:xcode4"
    pchheader "../pch/neptunepch.h"
    
  filter "action:vs*"
    pchheader "neptunepch.h"
    pchheader "neptunepch.cpp"

  filter "configurations:Debug"
    defines "NEPTUNE_DEBUG"
		symbols "On"

  filter "configurations:Release"
    defines "NEPTUNE_RELEASE"
		optimize "On"
	
  filter "configurations:Dist"
    defines "NEPTUNE_DIST"
		optimize "Full"
