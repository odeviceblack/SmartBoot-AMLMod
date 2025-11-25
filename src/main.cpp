#include "main.hpp"
#include "SkipSocialClub.hpp"
#include "StartGame.hpp"

MYMODCFGNAME(com.deviceblack.gtasa.SmartBoot, SmartBoot, 2.1, DeviceBlack, SmartBoot)
NEEDGAME(com.rockstargames.gtasa)

void* h_lib_scand = nullptr;
void* h_lib_gtasa = nullptr;

void safeClear(JNIEnv* env)
{
	if(env->ExceptionCheck())
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
	}
}

ON_MOD_PRELOAD()
{
	logger->SetTag("SmartBoot");

	h_lib_scand = aml->GetLibHandle("libSCAnd.so");
	logger->Info("libSCAnd.so: 0x%" PRIXPTR, (uintptr_t)h_lib_scand);

	h_lib_gtasa = aml->GetLibHandle("libGTASA.so");
	logger->Info("libGTASA.so: 0x%" PRIXPTR, (uintptr_t)h_lib_gtasa);
}

ON_MOD_LOAD()
{
	if(h_lib_scand)
	{
		if(cfg->GetBool("SkipSocialClub", true, "SCAndSkip"))
			hookSocialClub();
	}
	else logger->Info("SocialClub screen skip unavailable");

	if(h_lib_gtasa)
	{
		const char* mode = cfg->GetString("Mode", "auto", "SmartBoot");
		const char* slots = cfg->GetString("Saves", "GTASAsf9.b GTASAsf10.b", "SmartBoot");
		startGameProcess(mode, slots);
	}
	else logger->Info("GTASA quick-start unavailable");
}
