#include "StartGame.hpp"
#include "main.hpp"
#include <unordered_map>
#include <sstream>

int start_mode = 0;
std::vector<std::string> savegames;

void (*StartGameScreen_OnNewGameCheck)() = nullptr;
void (*StartGameScreen_OnLoadGame)() = nullptr;
void (*Menu_LoadSlot)(int) = nullptr;
void (*Menu_Resume)() = nullptr;

static const std::unordered_map<std::string, int> SLOT_MAP = {
	{"GTASAsf1.b",	0},
	{"GTASAsf2.b",	1},
	{"GTASAsf3.b",	2},
	{"GTASAsf4.b",	3},
	{"GTASAsf5.b",	4},
	{"GTASAsf6.b",	5},
	{"GTASAsf9.b",	8},
	{"GTASAsf10.b",	9},
};

int GetStartMode(const char* mode)
{
	if(!mode) return 0;

	static const std::unordered_map<std::string, int> map = {
		{"none",		0},
		{"newgame",		1},
		{"loadgame",	2},
		{"loadslot",	3},
		{"auto",		4},
		{"auto2",		5},
	};

	auto it = map.find(mode);
	return (it != map.end()) ? it->second : 0;
}

int GetSlotIndex(const std::string& slot)
{
	auto it = SLOT_MAP.find(slot);
	return (it != SLOT_MAP.end()) ? it->second : -1;
}

std::vector<std::string> SplitSlots(const char* slotsStr)
{
	std::vector<std::string> result;
	if(!slotsStr) return result;

	std::istringstream ss(slotsStr);
	std::string token;

	while(ss >> token)
		result.push_back(token);

	return result;
}

int FindFirstExistingSlot()
{
	std::string basePath = std::string(aml->GetConfigPath()) + "../files/";

	for(const std::string& slot : savegames)
	{
		std::string full = basePath + slot;

		if(fs::exists(full))
			return GetSlotIndex(slot);
	}

	return -1;
}

DECL_HOOKv(MainMenuScreen_Update, void* self, float delta)
{
	MainMenuScreen_Update(self, delta);

	static int start_status = 0;
	if(start_status == 2)
		return;

	if(start_status == 1)
	{
		start_status = 2;
		// Menu_Resume(); Not working
		return;
	}

	start_status = 2;

	int existingSlot = FindFirstExistingSlot();

	switch(start_mode)
	{
		case 1:
			StartGameScreen_OnNewGameCheck();
			break;

		case 2:
			StartGameScreen_OnLoadGame();
			break;

		case 3:
			if(existingSlot >= 0)
			{
				start_status = 1;
				Menu_LoadSlot(existingSlot);
			}
			break;

		case 4:
			if(existingSlot >= 0)
			{
				start_status = 1;
				Menu_LoadSlot(existingSlot);
				break;
			}
			StartGameScreen_OnNewGameCheck();
			break;

		case 5:
			if(existingSlot >= 0)
			{
				start_status = 1;
				Menu_LoadSlot(existingSlot);
				break;
			}
			StartGameScreen_OnLoadGame();
			break;
	}
}

void StartGameProcess(const char* mode, const char* slots)
{
	start_mode = GetStartMode(mode);
	savegames = SplitSlots(slots);

	SETSYM_TO(StartGameScreen_OnNewGameCheck, h_libGTASA, "_ZN15StartGameScreen14OnNewGameCheckEv");
	SETSYM_TO(StartGameScreen_OnLoadGame, h_libGTASA, "_ZN15StartGameScreen10OnLoadGameEv");
	SETSYM_TO(Menu_LoadSlot, h_libGTASA, "_Z13Menu_LoadSloti");
	SETSYM_TO(Menu_Resume, h_libGTASA, "_Z11Menu_Resumev");

	HOOKSYM(MainMenuScreen_Update, h_libGTASA, "_ZN14MainMenuScreen6UpdateEf");
}
