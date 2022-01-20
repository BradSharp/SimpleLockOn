#include "Console.h"
#include "Settings.h"
#include "PatternScanner.h"
#include "skse64_common/SafeWrite.h"
#include "skse64/GameAPI.h"

namespace Tralala
{
	uintptr_t g_consoleMgrAddr = 0;

	void ConsoleGetAddresses()
	{
		g_consoleMgrAddr = g_console.GetUIntPtr();
	}

	ConsoleManager* ConsoleManager::GetSingleton()
	{
		return *(ConsoleManager * *)g_consoleMgrAddr;
	}

	static ObScriptCommand* s_hijackedCommand = nullptr;

	static bool Cmd_SetLockOnVariable_Execute(void* paramInfo, void* scriptData, TESObjectREFR* thisObj, void* containingObj, void* scriptObj, void* locals, double* result, void* opcodeOffsetPtr)
	{
		ObScriptCommand::ScriptData* scriptDataEx = (ObScriptCommand::ScriptData*)scriptData;
		ObScriptCommand::StringChunk* strChunk = (ObScriptCommand::StringChunk*)scriptDataEx->GetChunk();
		std::string name = strChunk->GetString();

		ObScriptCommand::IntegerChunk* intChunk = (ObScriptCommand::IntegerChunk*)strChunk->GetNext();
		int val = intChunk->GetInteger();

		if (Settings::Set(name.c_str(), val))
			Console_Print("> set Settings::%s = %d", name.c_str(), val);
		else
			Console_Print("> (Error) Settings::%s is not found.", name.c_str());

		return true;
	}
}

namespace ConsoleCommand
{

	uintptr_t g_firstConsoleAddr = 0;

	void GetAddress()
	{
		g_firstConsoleAddr = g_firstConsoleCommand.GetUIntPtr();
	}

	void Register()
	{
		_MESSAGE("ConsoleCommand::Register()");

		for (Tralala::ObScriptCommand * iter = (Tralala::ObScriptCommand*)g_firstConsoleAddr; iter->opcode < kObScript_NumConsoleCommands + kObScript_ConsoleOpBase; ++iter)
		{
			if (!strcmp(iter->longName, "ToggleESRAM")) // unused
			{
				Tralala::s_hijackedCommand = iter;
				break;
			}
		}

		Tralala::ObScriptCommand cmd = *Tralala::s_hijackedCommand;
		static Tralala::ObScriptParam params[] = {
			{ "String", 0, 0 },
			{ "Integer", 1, 0 }
		};
		cmd.longName = "SetLockOnVariable";
		cmd.shortName = "setlockonvar";
		cmd.helpText = "";
		cmd.needsParent = 0;
		cmd.numParams = 2;
		cmd.params = params;
		cmd.execute = &Tralala::Cmd_SetLockOnVariable_Execute;
		cmd.eval = nullptr;
		cmd.flags = 0;

		SafeWriteBuf((uintptr_t)Tralala::s_hijackedCommand, &cmd, sizeof(cmd));
	}
}
