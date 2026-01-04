project "renderer"
	language "C++"
	architecture "x64"
	kind "SharedLib"
	staticruntime "off"
	targetdir ("%{wks.location}/OUTPUT/"..outputdir .. "/bin" .. "/%{prj.name}")
	objdir ("%{wks.location}/OUTPUT/"..outputdir.."/bin-int" .."/%{prj.name}")
	
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
		thirdparty.."/tiny_obj_loader/**.h",
		thirdparty.."/tiny_obj_loader/**.cc",
		thirdparty.."/spdlog/**.h",
		thirdparty.."/stb/**.h",
		thirdparty.."/stb/**.cpp",
		thirdparty.."/json.hpp",

	}
	includedirs
    {
	   "./",
       thirdparty.."/glm",
       thirdparty.."/imgui",
       thirdparty.."/tiny_obj_loader",
       thirdparty.."/glad/include",
       thirdparty.."/glfw/include",
       thirdparty.."/stb",
       thirdparty.."/spdlog",
       thirdparty.."/",
    }

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_JN_ENGINE",
		}
	
	links
	{
		"GLFW",
		"Glad",
	}
	
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath}  %{wks.location}/OUTPUT/"..outputdir .."/bin" ..  "/editor ")
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"