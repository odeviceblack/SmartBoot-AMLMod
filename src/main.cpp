#include "main.hpp"
#include "SCAnd/SkipSocialClub.hpp"
#include "Game/FastPlay.hpp"

MYMODCFGNAME(com.deviceblack.gtasa.SmartBoot, SmartBoot, 2.1, DeviceBlack, SmartBoot)
NEEDGAME(com.rockstargames.gtasa)

void* h_lib_scand = nullptr;
void* h_lib_gtasa = nullptr;

ON_MOD_PRELOAD()
{
	logger->SetTag("SmartBoot");
	
	#ifdef LOG_IN_FILE
		logger->SetFile(aml->GetConfigPath(), "../mods/SmartBoot.txt");
		logger->EnableFileLogging(true);
	#endif

	h_lib_scand = aml->GetLibHandle("libSCAnd.so");
	logger->Info("libSCAnd.so: 0x%" PRIXPTR, (uintptr_t)h_lib_scand);

	h_lib_gtasa = aml->GetLibHandle("libGTASA.so");
	logger->Info("libGTASA.so: 0x%" PRIXPTR, (uintptr_t)h_lib_gtasa);
}

ON_MOD_LOAD()
{
	bool skip_sc = cfg->GetBool("SkipSocialClub", true, "SCAndSkip");
	const char* start_mode = cfg->GetString("Mode", "auto", "SmartBoot");
	const char* savegames = cfg->GetString("Saves", "GTASAsf9.b GTASAsf10.b", "SmartBoot");

	if(skip_sc)
	{
		if(!h_lib_scand)
			return logger->Info("SocialClub screen skip unavailable");

		SkipSocialClub::Init();
	}

	if(!h_lib_gtasa)
		return logger->Info("Something went wrong when starting the mod");

	FastPlay::Init(start_mode, savegames);
}
