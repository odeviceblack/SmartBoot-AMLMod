#include "SkipSocialClub.hpp"
#include "FullScreen.hpp"
#include "../main.hpp"

namespace SkipSocialClub
{
	inline void trySignInOffline(void* self)
	{
		void(*signInOffline)(void*) = nullptr;
		SETSYM_TO(signInOffline, h_lib_scand, "_ZN10SocialClub13signInOfflineEv");
	
		if(signInOffline)
			signInOffline(self);
	}
	
	DECL_HOOKv(socialClub_LoadScreen, void* self)
	{
		FullScreen::Hide();
		trySignInOffline(self);
	}
	
	void Init()
	{
		HOOKSYM(socialClub_LoadScreen, h_lib_scand, "_ZN10SocialClub10LoadScreenEv");
	}
}
