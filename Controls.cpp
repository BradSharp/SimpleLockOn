#include "Controls.h"
#include "PatternScanner.h"
#include "skse64/GameInput.h"

namespace Tralala
{
	uintptr_t g_playerControlsAddr = 0;
	uintptr_t g_menuControlsAddr = 0;
	uintptr_t g_inputManagerAddr = 0;

	void ControlsGetAddresses()
	{
		g_playerControlsAddr = (uintptr_t)PlayerControls::GetSingleton();
		g_menuControlsAddr = (uintptr_t)MenuControls::GetSingleton();
		g_inputManagerAddr = (uintptr_t)InputManager::GetSingleton();
	}

	MenuControls * MenuControls::GetSingleton()
	{
		return *(MenuControls**)g_menuControlsAddr;
	}

	InputManager * InputManager::GetSingleton()
	{
		return *(InputManager**)g_inputManagerAddr;
	}

	PlayerControls * PlayerControls::GetSingleton()
	{
		return *(PlayerControls**)g_playerControlsAddr;
	}
}