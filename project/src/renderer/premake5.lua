project "renderer"
	language "C++"
	architecture "x64"
	kind "SharedLib"
	staticruntime "off"
	targetdir ("%{wks.location}/OUTPUT"..outputdir .. "/bin" .. "/%{prj.name}")
	objdir ("%{wks.location}/OUTPUT"..outputdir.."/bin-int" .."/%{prj.name}")
	
	files
	{
		"**.cpp",
		"**.h",
		"**.hpp",

		"**.lua",
		thirdparty.."/glm/**.cpp",
		thirdparty.."/glm/**.hpp",
		thirdparty.."/glm/**.h",
		thirdparty.."/imgui/**.cpp",
		thirdparty.."/imgui/**.hpp",
		thirdparty.."/imgui/**.h",
	}
	includedirs
    {
       thirdparty.."/glm",
       thirdparty.."/imgui",
       thirdparty.."/glad/include",
       thirdparty.."/glfw/include",
    }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 

		}
	
	links
	{
		"GLFW",
		"Glad",
	}
	
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath}  %{wks.location}/"..outputdir .."/bin/" ..  "/Editor ")
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"