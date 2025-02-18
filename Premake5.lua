local ProjectRelativeSDKSRoot = "$(ProjectDir)../../../shared/src/SDKS"
local ProjectRelativeFuetEngineRoot = "$(ProjectDir)../../../shared/src/FuetEngineFramework/FuetEngine"
local SDKSRoot = "$(FuetEngineProjectsDev)/shared/src/sdks"
local FuetEngineRoot = "$(FuetEngineProjectsDev)/shared/src/FuetEngineFramework/FuetEngine"
local ProjectRelativeFinalDataRoot = "$(ProjectDir)../../data/finaldata/HD"
	
workspace "ClassViewerAndAnalyzer"
    configurations { "Debug", "Release" }
    platforms { "Win32", "x64" }
	location "build" -- Where generated files (like Visual Studio solutions) will be stored
	filter {"platforms:Win32"}
	architecture "x86"
	filter {"platforms:x64"}
	architecture "x86_64"
	filter {}

project "ClassViewer"
    kind "WindowedApp" -- Change to "SharedLib" for a shared library
    language "C++"
    cppdialect "C++17"
    targetdir("$(ProjectDir)../exe/%{cfg.platform}/%{cfg.buildcfg}") -- Output directory for binaries
    objdir("$(ProjectDir)../obj/%{cfg.platform}/%{cfg.buildcfg}") -- Output directory for intermediate files
	characterset("ASCII")
	debugdir(ProjectRelativeFinalDataRoot)

	-- Specify the root directory of the library
    local sourceRoot = os.getcwd() .. "/src"

    -- Recursively include all .cpp and .h files from the sourceRoot directory
    files {
        sourceRoot .. "/**.cpp",
        sourceRoot .. "/**.inl",		
        sourceRoot .. "/**.h"
    }
	
	excludes {
		sourceRoot .. "/**dsi_app.*",
		sourceRoot .. "/**CPlayer_Backup.*",
	}
	
	filter { "system:windows" }
		excludes { 
			"**/Linux/**",
			"**/Cocos2D_X/**",
			"**/Android/**",
		}

	filter {} -- Reset filter


	-- specific defines for this project
	defines {
		"_MBCS" ,
		"NULL=0",
		"ALUT_BUILD_STATIC",		
	}
	
	filter { "system:windows" }
		defines { 
			"WIN32", 
			"_WIN32",
			"_WINDOWS",
		}
	filter {} -- Reset filter

    -- Add include directories (sourceRoot is included by default)
    includedirs {
		"$(ProjectDir)../src",
		FuetEngineRoot .. "/src",
		FuetEngineRoot .. "/../extensions",
		SDKSRoot .. "/externals/freeimage/Dist",
		SDKSRoot .. "/externals/libconfigPortable/src",
		SDKSRoot .. "/externals/OpenAL_1.1_SDK/include",
		SDKSRoot .. "/externals/OpenAL_1.1_SDK/include/al",
		SDKSRoot .. "/externals/freealut/include",
		SDKSRoot .. "/externals/physfs/",
		SDKSRoot .. "/externals/OpenGL",
    }

    -- Configuration-specific settings
    filter "configurations:*Debug"
        defines { "DEBUG" }
        symbols "On" -- Generate debug symbols
		staticruntime "off"
		runtime "Debug"
	filter {}

    filter "configurations:*Release"
        defines { "NDEBUG" }
        optimize "On" -- Enable optimizations
		staticruntime "off"
		runtime "Release"
	filter {}

	filter "configurations:DEV_Debug or configurations:DEV_Release or configurations:DS_PC_Debug"
        defines { "DEVELOPMENT" }
	filter {}
	
	filter "configurations:DEV_WITH_SOUND_Debug"
        defines { "DEVELOPMENT_WITH_SOUND" }
	filter {}
	
	filter "configurations:PLAYTEST_*"
        defines { "PLAYTEST" }
	filter {}

	disablewarnings
	{
		"4091",
	}

	-- Library directories common for all configurations
	libdirs
	{
		FuetEngineRoot .. "/lib/%{cfg.platform}/DEV_%{cfg.buildcfg}",
		SDKSRoot .. "/externals/FreeImage/Dist/%{cfg.platform}/%{cfg.buildcfg}",
		-- SDKSRoot .. "/externals/physfs/build/%{cfg.buildcfg}/%{cfg.buildcfg}"",
		SDKSRoot .. "/externals/libConfigPortable/lib/%{cfg.platform}/%{cfg.buildcfg}",
		SDKSRoot .. "/externals/freealut/lib/%{cfg.platform}/%{cfg.buildcfg}",
	}

	filter {"platforms:Win32"}
		libdirs
		{
			SDKSRoot .. "/externals/OpenAL_1.1_SDK/libs/Win32/",
		}
	filter {"platforms:x64"}
		libdirs
		{
			SDKSRoot .. "/externals/OpenAL_1.1_SDK/libs/Win64/",
		}
	filter {}
	
	if _useSound then
		libdirs
		{
			ProjectRelativeFuetEngineRoot .. "/lib/%{cfg.platform}/DEV_WITH_SOUND_%{cfg.buildcfg}"
		}
	else
		libdirs
		{
			ProjectRelativeFuetEngineRoot .. "/lib/%{cfg.platform}/DEV_%{cfg.buildcfg}"
		}
	end
	

	links 
	{
		"FuetEngine.lib",
		"ddraw.lib",
		"dxguid.lib",
		"opengl32.lib",
		"glu32.lib",
		"freeimage.lib",
		"libconfigPortable.lib",
		"OpenAL32.lib",
		"alut.lib",
		-- "physfs.lib",
		"comctl32.lib",
		"rpcrt4.lib",
		"odbc32.lib",
	}
	
	postbuildcommands
	{
		"{COPYFILE} " .. SDKSRoot .. "/Externals/FreeImage/Dist/%{cfg.platform}/%{cfg.buildcfg}/FreeImage.dll " .. ProjectRelativeFinalDataRoot
	}
