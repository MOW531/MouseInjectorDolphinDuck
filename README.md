# Mouse Injector for Dolphin 5.0, DuckStation, Mupen64Plus (RetroArch/BizHawk 2.8), BSNES, PCSX2

An external app that injects cursor input into game memory.

### *If you have a game request, please go to the 'Discussions' tab and post it!*

## Supported Emulators
| Emulator/Frontend | Version | Executable name (case sensitive) |
| --- | :---: | :---: |
| Dolphin | 5.0 and up | dolphin.exe |
| DuckStation | 0.1-5437 | duckstation-qt-x64-ReleaseLTCG.exe |
| RetroArch (Mupen64Plus-Next core) | 1.14.0 (2.4-Vulkan bc24153)| retroarch.exe |
| BizHawk (N64, Mupen64Plus) | 2.8 | EmuHawk.exe |
| BSNES | v115 | bsnes.exe |
| PCSX2 Nightly | 1.7.4126 | pcsx2-qtx64-avx2.exe |
* NOTE: Versions given are the latest that have been tested working, may work with newer
* NOTE: PCSX2 will only hook with **BIOS versions 5XXXX and up**.

## How to Use
1. Start emulator first
2. Start MouseInjector, read initial information then press ctrl+1
3. Make sure game is running and press '4' to hook into the process
    1. If game is supported then the mouse will be captured at the position it was at when hooked
        * You will be <b><u>unable</u></b> to use the mouse elsewhere while it is hooked, press 4 to unhook
        * Some games depend on post startup values/addresses so hook may not happen immediately
            * DuckStation games usually will not hook until after the startup sequence
    2. Unsupported/broken games will not hook and mouse won't be captured
4. Adjust options with numbers 4-7 while in-game, ctrl+0 will lock the settings
* NOTE: The cursor still moves but gets moved back to it's initial hook position so windowed mode may not
work very well if you have also mapped the mouse buttons as you may click off the window. Fullscreen is
recommended and with dual-monitors it is recommended to put the cursor in the corner before hooking to
avoid clicking off the window.

## Supported Dolphin Titles (NTSC Only)
| Game Title | Mouse Support | Issues |
| --- | :---: | ----------- |
| TimeSplitters 2 | Fair | <sup>Camera/sentry modes not supported</sub> |
| TimeSplitters: Future Perfect | Poor | <sup>All non-first person modes are not supported</sub> |
| 007: NightFire | Poor | <sup>Vehicle mode is semi-functional - last level is broken</sub> |
| Serious Sam: Next Encounter | Fair | <sup>Vehicle/submarine interfaces are not supported</sub> |
| Medal of Honor: Frontline | Fair | <sup>Minecart level is broken</sub> |
| Medal of Honor: European Assault | Good | <sup>None</sub> |
| Medal of Honor: Rising Sun | Poor | <sup>Looking down scope while in turret mode is broken</sub> |
| Metal Arms | Good | <sup>Rat driving or rat turret may not work correctly<sup> |
| Call of Duty 2: Big Red One | Good | <sup>None</sub> |
| Die Hard: Vendetta | Fair | <sup>Sentry mode not supported</sub> |
| Trigger Man | Good | <sup>None</sub> |
| Geist | Fair | <sup> ** *Requires MMU be disabled for game in Dolphin* ** <br />Camera broken on elevators, truck sentry on motorcycle level broken</sub> |

## Supported DuckStation Titles
| Game Title | Serial | Mouse Support | Issues |
| --- | --- | :---: | ----------- |
| Men in Black: The Series - Crashdown (NTSC) | SLUS-01387 | Good | <sup>None</sub> |
| Codename: Tenka (NTSC) | SCUS-94409 | Fair | <sup>Strafe/Lean must be set to R2 in-game for strafe to work without holding the button</sub> |
| Medal of Honor: Underground (NTSC) | SLUS-01270 | Fair | <sup>Machine Gun sentry doesn't always work (depends on objects in line of sight). Sidecar gun in 6-3 not supported.</sub> |
| Revolution X (NTSC) | SLUS-00012 | Good | <sup>None</sub> |
| Armorines: Project S.W.A.R.M. | SLUS-01022 | Fair | <sup>Not fully tested</sub> |
| Resident Evil: Survivor (NTSC-U) | SLUS-01087 | Good | <sup>None</sub> |
| The Note (PAL) | SLES-00749 | Good | <sup>Not fully tested</sub> |
| Echo Night (NTSC-U) | SLUS-00820 | Good | <sup>Not fully tested</sub> |
| Shadow Tower (NTSC-U) | SLUS-00863 | Good | <sup>Not fully tested</sub> |
| South Park (NTSC-U) | SLUS-00936 | Good | <sup>Supplied cheats recommended (SouthPark_SLUS-00936.cht), Not fully tested</sub> |
* Importing cheat files in DuckStation: 'Tools->Cheat Manager->Cheat List->Import->From File'

## Supported Mupen64Plus(RetroArch)/BizHawk 2.8 Titles
| Game Title | Mouse Support | Issues |
| --- | :---: | ----------- |
| GoldenEye: 007 (NTSC) | Fair | <sup>None</sub> |
| Sin and Punishment (J) | Good | <sup>Not fully tested</sub> |

## Supported BSNES Titles
| Game Title | Mouse Support | Issues |
| --- | :---: | ----------- |
| Pac-Man 2: The New Adventures | Good | <sup>Not fully tested</sub> |
| Timon & Pumbaa's Jungle Games | Good | <sup>None</sub> |
| The Untouchables | Good | <sup>Crosshair shooting sections only</sub> |
| R-Type III: The Third Lightning | Good | <sup>Not fully tested</sub> |
* BSNES TIP: Press F12 to toggle mouse capture and it will hide the cursor.
* NOTE: At the moment BSNES may unhook after minimizing, menuing, pausing emulation, or loading/saving savestates. Requires restarting BSNES and MouseInjector.

## Supported PCSX2 Titles
| Game Title | Serial | Mouse Support | Issues | In-game Options |
| ------ | --- | :---: | ----------- | ----------- |
| Red Dead Revolver (NTSC-U) | SLUS-20500 | Fair | <sup>Gatling guns and final scene may break if game is loaded from memory card after a shutdown.  **Fix below*</sub> | <sup>Target Mode: Toggle</sub> |
| Time Crisis II (NTSC-U) | SLUS-20219 | Good | <sup>Not fully tested</sub> | |
| Vampire Night (NTSC-U) | SLUS-20221 | Good | <sup>Not fully tested</sub> | |
| Gunslinger Girl Vol. 1 (Japan)| SLPS-25343 | Fair | <sup>Not fully tested</sub> | |
| Darkwatch (NTSC-U) | SLUS-21042 | Good | <sup>***Requires use of supplied cheat file to work (327053E8.pnach)*** Horse aiming is not quite right but is usable.</sub> | |
| Black (NTSC-U) | SLUS-21376 | Good | <sup>Not fully tested</sub> | |
| Urban Chaos: Riot Response (NTSC-U) | SLUS-21390 | Good | <sup>Not fully tested</sub> | <sup>Auto-Center: No</sub> |
| 007: Agent Under Fire (NTSC-U) | SLUS-20265 | Good | <sup>Mouse movement warps camera while paused and during in-game cutscenes, aim-lock not disabled on auto-scroller levels</sub> | |
| Quake III: Revolution (NTSC-U) | SLUS-20167 | Good | <sup>None</sub> | <sup>Auto Center: No, Auto Aiming: no (only available from main menu options) |
| 50 Cent: Bulletproof (NTSC-U) | SLUS-21315 | Good | <sup>Not fully tested</sub> | <sup>Camera->Aim Assist: Off</sub> |
| Call of Duty: Finest Hour (NTSC-U) | SLUS-20725 | Good | <sup>None</sub> | <sup>Aim Assist: Off</sub> |
| Cold Winter (NTSC-U) | SLUS-20845 | Good | <sup>Split-screen mode not supported</sub> | <sup>Profile options - Aim Assist: Off</sub> |
| Medal of Honor: Vanguard (NTSC-U) | SLUS-21597 | Good | <sup>Multiplayer mode not supported</sub> | |
| Mercenaries: Playground of Destruction (NTSC-U) | SLUS-20932 | Fair | <sup>***Requires cheat file (23510F99.pnach) to disable aim-assist***, x-axis in normal vehicles not supported</sub> | |
* NOTE: PCSX2 will only hook with **BIOS versions 5XXXX and up**.
* NOTE: Some aspects may break when a game is started with overclocking. Requires testing.
* PCSX2 Settings: **Disable** *'Settings->Interface->Double-Click Toggles Fullscreen'* | **Enable** *'Settings->Interface->Hide Cursor In Fullscreen'*
* RDR Gatling/Final Scene Fix: Start a new game on a new name. When in-game, pause and quit back to menu. Reload your main save.
* Place cheat files in 'cheats/PS2' folder in the main PCSX2 directory. In PCSX2 go to 'Settings->Emulation' and tick 'Enable Cheats'.

# ManyMouse

ManyMouse is Copyright (c) 2005-2012 Ryan C. Gordon and others. https://icculus.org/manymouse/
