#include "StartGame.hpp"
#include "main.hpp"

#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <sstream>

namespace fs = std::filesystem;

namespace
{
	int start_mode = 0;
	int start_slot = -1;

	std::vector<std::string> save_slots;

	const std::unordered_map<std::string, int> slot_map = {
		{"GTASAsf1.b", 0},
		{"GTASAsf2.b", 1},
		{"GTASAsf3.b", 2},
		{"GTASAsf4.b", 3},
		{"GTASAsf5.b", 4},
		{"GTASAsf6.b", 5},
		{"GTASAsf9.b", 8},
		{"GTASAsf10.b", 9}
	};

	void (*startGameScreen_onNewGameCheck)() = nullptr;
	void (*startGameScreen_onLoadGame)() = nullptr;
	void (*mainMenuScreen_onResume)(void*) = nullptr;

	inline int getStartMode(const char* mode)
	{
		if(!mode) return 0;

		static const std::unordered_map<std::string, int> map = {
			{"none",0},
			{"newgame",1},
			{"loadgame",2},
			{"loadslot",3},
			{"auto",4},
			{"auto2",5}
		};

		auto it = map.find(mode);
		return (it != map.end()) ? it->second : 0;
	}

	inline int getSlotIndex(const std::string& slot)
	{
		auto it = slot_map.find(slot);
		return (it != slot_map.end()) ? it->second : -1;
	}

	inline std::vector<std::string> splitSlots(const char* slots_str)
	{
		std::vector<std::string> out;
		if(!slots_str) return out;

		std::istringstream ss(slots_str);
		std::string tmp;

		while(ss >> tmp)
			out.push_back(tmp);

		return out;
	}

	void findFirstExistingSlot()
	{
		std::string base_path = std::string(aml->GetConfigPath()) + "../files/";

		for(const std::string& slot : save_slots)
		{
			std::string full = base_path + slot;

			if(fs::exists(full))
			{
				start_slot = getSlotIndex(slot);

				if(start_slot < 8)
				{
					std::string dest = base_path + "GTASAsf10.b";

					try
					{
						fs::copy_file(full, dest, fs::copy_options::overwrite_existing);
					}
					catch(const fs::filesystem_error& e)
					{
						logger->Error("Error copying save: %s", e.what());
						start_slot = -1;
					}
				}
				break;
			}
		}
	}

	DECL_HOOKv(mainMenuScreen_Update, void* self, float delta)
	{
		mainMenuScreen_Update(self, delta);
		if(start_mode == 0) return;

		switch(start_mode)
		{
			case 1:
				startGameScreen_onNewGameCheck();
				break;

			case 2:
				startGameScreen_onLoadGame();
				break;

			case 3:
				if(start_slot != -1)
					mainMenuScreen_onResume(self);
				break;

			case 4:
				if(start_slot == -1)
					startGameScreen_onNewGameCheck();
				else
					mainMenuScreen_onResume(self);
				break;

			case 5:
				if(start_slot == -1)
					startGameScreen_onLoadGame();
				else
					mainMenuScreen_onResume(self);
				break;
		}

		start_mode = 0;
	}

	DECL_HOOKv(menu_LoadSlot, int slot)
	{
		if(start_slot == -1)
			return menu_LoadSlot(slot);

		menu_LoadSlot(start_slot);
		start_slot = -1;
	}
}

void startGameProcess(const char* mode, const char* slots)
{
	start_mode = getStartMode(mode);
	save_slots = splitSlots(slots);

	SETSYM_TO(startGameScreen_onNewGameCheck, h_lib_gtasa, "_ZN15StartGameScreen14OnNewGameCheckEv");
	SETSYM_TO(startGameScreen_onLoadGame, h_lib_gtasa, "_ZN15StartGameScreen10OnLoadGameEv");
	SETSYM_TO(mainMenuScreen_onResume, h_lib_gtasa, "_ZN14MainMenuScreen8OnResumeEv");

	HOOKSYM(mainMenuScreen_Update, h_lib_gtasa, "_ZN14MainMenuScreen6UpdateEf");

	if(start_mode >= 3)
	{
		findFirstExistingSlot();
		HOOKSYM(menu_LoadSlot, h_lib_gtasa, "_Z13Menu_LoadSloti");
	}
}
