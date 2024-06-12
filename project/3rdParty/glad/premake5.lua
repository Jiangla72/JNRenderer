project "Glad"
	language "C"
	kind "StaticLib"

	targetdir ("%{wks.location}/OUTPUT"..outputdir .. "/bin" .. "/%{prj.name}")
	objdir ("%{wks.location}/OUTPUT"..outputdir.."/bin-int" .."/%{prj.name}")
	includedirs
	{
		"src",
		"include",
	}
	
	files
	{
		"**.h",
		"**.c",

		"**.lua",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"