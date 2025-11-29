# SmartBoot-AMLMod

![Downloads](https://img.shields.io/github/downloads/odeviceblack/SmartBoot-AMLMod/total.svg)
![Latest Tag](https://img.shields.io/github/v/tag/odeviceblack/SmartBoot-AMLMod)
![armeabi-v7a](https://img.shields.io/badge/armeabi--v7a-available-brightgreen)
![arm64-v8a](https://img.shields.io/badge/arm64--v8a-available-brightgreen)

**SmartBoot** is an advanced startup enhancement for **GTA San Andreas Android**.  
It skips unnecessary screens (Social Club, EULA), ensures fullscreen is properly applied, and adds a fully configurable boot system that decides how the game should start.

Instead of stopping on menus or prompts, SmartBoot can automatically:

- start a new game,
- load the default save menu,
- try loading specific save slots from a custom list,
- or fall back to a new game if no save is found.

Fast, clean, and interruption-free — the game boots exactly the way you want.

![Demonstration](media/video.gif)

---

## ⚙️ Features

- Skips Social Club login
- Configurable startup behavior via `SmartBoot.ini`  
- Supported modes:
  - **none** – no action  
  - **newgame** – start a new game  
  - **loadgame** – open the load menu  
  - **loadslot** – load a specific save slot  
  - **auto** – load first valid save or start a new game  
  - **auto2** – load first valid save or open the load menu  

---

## ⚙️ Configuration (`SmartBoot.ini`)

Example:

```
[SCAndSkip]
SkipSocialClub=1

[SmartBoot]
Mode=auto
Saves=GTASAsf9.b GTASAsf10.b

# GTASAsf7.b and GTASAsf8.b are considered invalid for this mod.
# GTASAsf1.b through GTASAsf6.b are intentional user savegames.
# GTASAsf9.b and GTASAsf10.b are automatic game savegames.
```
---

## 📦 Installation

1. Download `libSmartBoot.so` or `libSmartBoot64.so`
2. Place the file in:  
   `/sdcard/Android_unprotected/data/com.rockstargames.gtasa/mods/`
3. Launch the game — the mod loads automatically.

[Go to latest version](https://github.com/odeviceblack/SmartBoot-AMLMod/releases/latest)
