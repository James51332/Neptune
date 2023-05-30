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
includeDirs["glm"] = "thirdparty/glm"
includeDirs["stb_image"] = "thirdparty/stb_image"
includeDirs["imgui"] = "thirdparty/imgui"
includeDirs["OpenFBX"] = "thirdparty/OpenFBX"
includeDirs["assimp"] = "thirdparty/assimp/assimp/include"
includeDirs["entt"] = "thirdparty/entt/single_include"

libDirs = {}
libDirs["assimp"] = "thirdparty/assimp/lib/%{cfg.shortname}"

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
   	"thirdparty/**.h",
    "thirdparty/**.inl",
    "thirdparty/**.hpp",
    "thirdparty/stb_image/**.cpp",
    "thirdparty/imgui/imgui/*.h",
    "thirdparty/imgui/imgui/*.cpp",
    "thirdparty/OpenFBX/**.cpp",
    "thirdparty/OpenFBX/**.c",
  	"pch/neptunepch.h",
   	"pch/neptunepch.cpp"
	}

	includedirs
	{
		"src",
  	"platform",
  	"pch"
	}
	
	sysincludedirs
	{
		includeDirs["spdlog"],
  	includeDirs["concurrentQueue"],
  	includeDirs["glm"],
  	includeDirs["stb_image"],
  	includeDirs["imgui"],
  	includeDirs["OpenFBX"],
  	includeDirs["assimp"],
   	includeDirs["entt"]
	}
	
  links
	{
		"assimp",
  	"zlibstatic"
	}
	
	libdirs
	{
		libDirs["assimp"]
  }

	filter "system:windows"
		systemversion "latest"
  
  filter "system:macosx"
  	files
  	{
      "platform/macos/**.cpp",
      "platform/macos/**.mm",
      "platform/macos/**.h",
      "platform/metal/**.cpp",
      "platform/metal/**.mm",
      "platform/metal/**.h"
    }
  
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

project "Editor"
	kind "ConsoleApp"
	
	debugdir (path.getabsolute("."))

	targetdir ("build/bin/neptune-" .. string.lower("%{cfg.system}") .. "/%{cfg.shortname}/")
  objdir ("build/bin-obj/neptune-" .. string.lower("%{cfg.system}") .. "/%{cfg.shortname}/")

	files
	{
		"editor/**.cpp",
		"editor/**.h"
	} 

	libdirs
	{
		libDirs["assimp"]
  }

	links
	{
		"Neptune",
  	"assimp",
   	"zlibstatic"
	}
	
	includedirs
	{
		"src",
    "platform",
    "pch"
	}

	sysincludedirs
	{
		"include",
  	includeDirs["spdlog"],
   	includeDirs["concurrentQueue"],
    includeDirs["glm"],
    includeDirs["stb_image"],
  	includeDirs["imgui"],
  	includeDirs["OpenFBX"],
  	includeDirs["assimp"],
  	includeDirs["entt"]
	}
	
  filter "system:windows"
    systemversion "latest"
    
  filter "system:macosx"
    links
    {
    	"Cocoa.framework",
    	"QuartzCore.framework",
  		"Metal.framework"
    }
    
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
