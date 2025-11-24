#include "SkipSocialClub.hpp"
#include "FullScreen.hpp"
#include "main.hpp"

DECL_HOOKv(SocialClub_LoadScreen, void* self)
{
	hideSystemUI();

	void (*signInOffline)(void*) = nullptr;
	SETSYM_TO(signInOffline, h_libSCAnd, "_ZN10SocialClub13signInOfflineEv");

	if(signInOffline)
		signInOffline(self);
}

void HookSocialClub()
{
	HOOKSYM(SocialClub_LoadScreen, h_libSCAnd, "_ZN10SocialClub10LoadScreenEv");
}
