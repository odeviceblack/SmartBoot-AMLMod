#include <mod/amlmod.h>
#include <inttypes.h>
#include <logger.h>

MYMOD(SCAndSkip, ExitSocialClub, 1.0, DeviceBlack)

//------------------------//
// Global variables
//------------------------//
void* h_SCAnd = nullptr;

//------------------------//
// Forward declarations
//------------------------//
void ExitSocialClub();
void SkipEULA();
void HookLoadScreen();

//------------------------//
// Hook da função SocialClub::LoadScreen
//------------------------//
DECL_HOOKv(SocialClub_LoadScreen, void* self)
{
	// Chama a função original
	SocialClub_LoadScreen(self);

	logger->Info("SocialClub_LoadScreen => Jumping to main menu!");

	// Chama SocialClub::signInOffline para pular autenticação
	void(*SocialClub_signInOffline)(void*) =
		(void(*)(void*)) aml->GetSym(h_SCAnd, "_ZN10SocialClub13signInOfflineEv");

	if(SocialClub_signInOffline)
		SocialClub_signInOffline(self);
	else
		logger->Error("Failed to locate SocialClub::signInOffline symbol!");
}

//------------------------//
// Função para pular a tela do EULA
//------------------------//
void SkipEULA()
{
	logger->Info("Looking for LegalScreenShown symbol...");

	uintptr_t LegalScreenShown = aml->GetSym(h_SCAnd, "LegalScreenShown");
	if(!LegalScreenShown)
	{
		logger->Error("Failed to locate LegalScreenShown symbol!");
		return;
	}

	// Força a variável para true para pular o EULA
	*(bool*) LegalScreenShown = true;
	logger->Info("Successfully skipped Social Club EULA screen.");
}

//------------------------//
// Hook principal
//------------------------//
void HookLoadScreen()
{
	uintptr_t symLoadScreen = aml->GetSym(h_SCAnd, "_ZN10SocialClub10LoadScreenEv");
	if(!symLoadScreen)
	{
		logger->Error("Failed to locate SocialClub::LoadScreen symbol!");
		return;
	}

	HOOK(SocialClub_LoadScreen, symLoadScreen);
	logger->Info("Hooked SocialClub::LoadScreen successfully.");
}

//------------------------//
// Inicialização do mod
//------------------------//
void ExitSocialClub()
{
	// Carrega a biblioteca libSCAnd.so
	logger->Info("Loading libSCAnd.so...");
	h_SCAnd = aml->GetLibHandle("libSCAnd.so");

	if(!h_SCAnd)
	{
		logger->Error("Failed to load libSCAnd.so!");
		return;
	}

	logger->Info("Successfully loaded libSCAnd.so => 0x%" PRIXPTR, (uintptr_t) h_SCAnd);

	// Pula a tela do EULA
	SkipEULA();

	// Hook LoadScreen para pular login
	HookLoadScreen();
}

//------------------------//
// Pré-carregamento do mod
//------------------------//
ON_MOD_PRELOAD()
{
	// Configurações do logger
	logger->SetTag(PROJECT_NAME_STR);

	/*
	 * logger->SetFile(aml->GetConfigPath(), "log_SCAndSkip.txt");
	 * logger->EnableFileLogging(true);
	 * logger->Info("File logging started!");
	 */

	// Inicializa o mod
	ExitSocialClub();
}
