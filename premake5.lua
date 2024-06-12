workspace "JNRenderer"
	language "C++"
	cppdialect "C++17"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
	}
	defines
		{
		}
	
thirdparty = "%{wks.location}/project/3rdParty"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
startproject "editor"
include "project/src/renderer"
include "project/src/editor"
group "ThirdParty"
	include "project/3rdParty/glfw"
	include "project/3rdParty/glad"

