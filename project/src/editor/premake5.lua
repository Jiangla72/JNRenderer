project "editor"
	language "C++"
	architecture "x64"
	kind "ConsoleApp"
	staticruntime "off"
	targetdir ("%{wks.location}/OUTPUT/"..outputdir .. "/bin" .. "/%{prj.name}")
	objdir ("%{wks.location}/OUTPUT/"..outputdir.."/bin-int" .."/%{prj.name}")
	
	files
	{
		"**.cpp",
		"**.h",
		"**.hpp",

		"**.lua",
	}
	includedirs
    {
       "../renderer/",
    }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 

		}
	
	links
	{
		"renderer",
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"