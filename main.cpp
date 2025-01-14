#include "skse64/PluginAPI.h"
#include "skse64_common/skse_version.h"
#include "skse64_common/BranchTrampoline.h"
#include <shlobj.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "Hooks.h"
#include "Utils.h"
#include "Scaleform.h"
#include "Settings.h"
#include "Console.h"
#include "Menus.h"
#include "Camera.h"
#include "ObjectRef.h"
#include "Controls.h"
#include "ScaleformUtil.h"
#include "Event.h"
#include "AddressRegistry.h"

IDebugLog				gLog;
PluginHandle			g_pluginHandle = kPluginHandle_Invalid;

void * g_moduleHandle = nullptr;

extern "C"
{

	SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,
		1,
		"Simple Lock On AE",
		"Sharpify",
		"support@example.com",
		SKSEPluginVersionData::kVersionIndependent_Signatures,
		{ RUNTIME_VERSION_1_6_353, 0 },	// compatible with 1.6.353
		0,
	};

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "SimpleLockOn plugin";
		info->version = 2;

		g_pluginHandle = skse->GetPluginHandle();

		if (skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");
			return false;
		}

		if (!g_branchTrampoline.Create(1024 * 64))
		{
			_ERROR("couldn't create branch trampoline. this is fatal. skipping remainder of init process.");
			return false;
		}

		if (!g_localTrampoline.Create(1024 * 64, g_moduleHandle))
		{
			_ERROR("couldn't create codegen buffer. this is fatal. skipping remainder of init process.");
			return false;
		}

		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)
	{

		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\SimpleLockOn.log");

		_MESSAGE("Load");

		Settings::Load();
		AddressRegistry::Initialize();

		Tralala::PlayerCameraGetAddress();
		Tralala::ConsoleGetAddresses();
		Tralala::ControlsGetAddresses();
		Tralala::EventGetAddresses();
		Tralala::MenusGetAddresses();
		Tralala::ObjectRefGetAddresses();
		Tralala::ScaleformUtilGetAddresses();
		Tralala::UtilsGetAddresses();
	
		MainHooks::GetAddresses();
		MainHooks::Install();

		ConsoleCommand::GetAddress();
		ConsoleCommand::Register();

		return true;
	}

}

