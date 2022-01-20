#pragma once
#include "versionlibdb.h"
#include "skse64/GameCamera.h"
#include "skse64/GameAPI.h"
//#include "skse64/Hooks_Camera.h"
//#include "skse64/Hooks_Event.h"

namespace AddressRegistry
{
	typedef uintptr_t Address;
	typedef void* RawAddress;
	typedef unsigned long long Offset;

	Address playerCamera = 0;
	Address playerControls = 0;
	Address menuControls = 0;
	Address inputManager = 0;
	Address consoleManager = 0;
	Address inputEventDispatcher = 0;
	Address eventDispatcher = 0;
	Address cameraHook = 0;

	void Initialize()
	{
		// TODO: Move this to CPP file
		playerCamera = (uintptr_t)PlayerCamera::GetSingleton();
		playerControls = (uintptr_t)PlayerControls::GetSingleton();
		menuControls = (uintptr_t)MenuControls::GetSingleton();
		inputManager = (uintptr_t)InputManager::GetSingleton();
		consoleManager = g_console.GetUIntPtr();
		inputEventDispatcher = (uintptr_t)InputEventDispatcher::GetSingleton();
		eventDispatcher = GetEventDispatcherList.GetUIntPtr();
		//cameraHook = kInstallCameraHook_Base.GetUIntPtr();
	}

}


