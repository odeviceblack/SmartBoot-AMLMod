#include "main.hpp"
#include "SkipSocialClub.hpp"
#include "StartGame.hpp"

MYMODCFGNAME(com.deviceblack.gtasa.SmartBoot, SmartBoot, 2.0, DeviceBlack, SmartBoot)
NEEDGAME(com.rockstargames.gtasa)

void* h_libSCAnd = nullptr;
void* h_libGTASA = nullptr;

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

	h_libSCAnd = aml->GetLibHandle("libSCAnd.so");
	logger->Info("libSCAnd.so: 0x%" PRIXPTR, (uintptr_t)h_libSCAnd);

	h_libGTASA = aml->GetLibHandle("libGTASA.so");
	logger->Info("libGTASA.so: 0x%" PRIXPTR, (uintptr_t)h_libGTASA);
}

ON_MOD_LOAD()
{
	if(h_libSCAnd)
	{
		if(cfg->GetBool("SkipSocialClub", true, "SCAndSkip"))
			HookSocialClub();
	}
	else logger->Info("It was not possible to skip the SocialClub screen");

	if(h_libGTASA)
	{
		const char* mode = cfg->GetString("Mode", "auto", "SmartBoot");
		const char* slots = cfg->GetString("Saves", "GTASAsf9.b GTASAsf10.b", "SmartBoot");
		StartGameProcess(mode, slots);
	}
	else logger->Info("Unable to process the quick start of the game");
}
