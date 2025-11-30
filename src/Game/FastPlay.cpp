#include "FastPlay.hpp"
#include "../main.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>   // access()

namespace FastPlay
{
	int start_mode = 0;
	int start_slot = -1;

	std::vector<std::string> save_slots;

	inline bool fileExists(const std::string& path)
	{
		return access(path.c_str(), F_OK) == 0;
	}

	inline bool copyFile(const std::string& src, const std::string& dst)
	{
		std::ifstream in(src, std::ios::binary);
		if(!in) return false;

		std::ofstream out(dst, std::ios::binary);
		if(!out) return false;

		out << in.rdbuf();
		return true;
	}

	inline int getStartMode(const char* mode)
	{
		if(!mode) return 0;

		if(strcmp(mode, "none") == 0) return 0;
		if(strcmp(mode, "newgame") == 0) return 1;
		if(strcmp(mode, "loadgame") == 0) return 2;
		if(strcmp(mode, "loadslot") == 0) return 3;
		if(strcmp(mode, "auto") == 0) return 4;
		if(strcmp(mode, "auto2") == 0) return 5;

		return 0;
	}

	inline int getSlotIndex(const std::string& slot)
	{
		if(slot == "GTASAsf1.b") return 0;
		if(slot == "GTASAsf2.b") return 1;
		if(slot == "GTASAsf3.b") return 2;
		if(slot == "GTASAsf4.b") return 3;
		if(slot == "GTASAsf5.b") return 4;
		if(slot == "GTASAsf6.b") return 5;
		if(slot == "GTASAsf9.b") return 8;
		if(slot == "GTASAsf10.b") return 9;
		return -1;
	}

	inline std::vector<std::string> splitSlots(const char* slots)
	{
		std::vector<std::string> out;
		if(!slots) return out;

		std::string tmp;
		while(*slots)
		{
			if(*slots == ' ')
			{
				if(!tmp.empty())
				{
					out.push_back(tmp);
					tmp.clear();
				}
			}
			else
				tmp.push_back(*slots);
			slots++;
		}
		if(!tmp.empty())
			out.push_back(tmp);

		return out;
	}

	void (*StartGameScreen_onNewGameCheck)() = nullptr;
	void (*StartGameScreen_onLoadGame)() = nullptr;
	void (*MainMenuScreen_onResume)(void*) = nullptr;

	void findFirstExistingSlot()
	{
		std::string base_path = std::string(aml->GetConfigPath()) + "../files/";

		for(const std::string& slot : save_slots)
		{
			std::string full = base_path + slot;

			if(fileExists(full))
			{
				start_slot = getSlotIndex(slot);

				if(start_slot < 8)
				{
					std::string dest = base_path + "GTASAsf10.b";

					if(!copyFile(full, dest))
					{
						logger->Error("Erro ao copiar save!");
						start_slot = -1;
					}
				}
				break;
			}
		}
	}


	DECL_HOOKv(MainMenuScreen_Update, void* self, float delta)
	{
		if(start_mode == 0)
			return MainMenuScreen_Update(self, delta);

		switch(start_mode)
		{
			case 1: StartGameScreen_onNewGameCheck(); break;
			case 2: StartGameScreen_onLoadGame(); break;

			case 3:
				if(start_slot != -1)
					MainMenuScreen_onResume(self);
				break;

			case 4:
				if(start_slot == -1)
					StartGameScreen_onNewGameCheck();
				else
					MainMenuScreen_onResume(self);
				break;

			case 5:
				if(start_slot == -1)
					StartGameScreen_onLoadGame();
				else
					MainMenuScreen_onResume(self);
				break;
		}

		start_mode = 0;
	}

	DECL_HOOKv(Menu_LoadSlot, int slot)
	{
		if(start_slot == -1)
			return Menu_LoadSlot(slot);

		Menu_LoadSlot(start_slot);
		start_slot = -1;
	}

	void Init(const char* mode, const char* slots)
	{
		start_mode = getStartMode(mode);
		save_slots = splitSlots(slots);

		SETSYM_TO(StartGameScreen_onNewGameCheck, h_lib_gtasa, "_ZN15StartGameScreen14OnNewGameCheckEv");
		SETSYM_TO(StartGameScreen_onLoadGame, h_lib_gtasa, "_ZN15StartGameScreen10OnLoadGameEv");
		SETSYM_TO(MainMenuScreen_onResume, h_lib_gtasa, "_ZN14MainMenuScreen8OnResumeEv");

		HOOKSYM(Menu_LoadSlot, h_lib_gtasa, "_Z13Menu_LoadSloti");

		if(start_mode > 0)
		{
			findFirstExistingSlot();
			HOOKSYM(MainMenuScreen_Update, h_lib_gtasa, "_ZN14MainMenuScreen6UpdateEf");
		}
	}
}
