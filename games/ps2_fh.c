//===========================================================
// Mouse Injector for Dolphin
//==========================================================================
// Copyright (C) 2019-2020 Carnivorous
// All rights reserved.
//
// Mouse Injector is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, visit http://www.gnu.org/licenses/gpl-2.0.html
//==========================================================================
#include <stdint.h>
#include "../main.h"
#include "../memory.h"
#include "../mouse.h"
#include "game.h"

#define FH_CAMY 0x371C70
#define FH_CAMX 0x371C74

static uint8_t PS2_FH_Status(void);
static void PS2_FH_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Fugitive Hunter - War on Terror",
	PS2_FH_Status,
	PS2_FH_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_FUGITIVEHUNTER = &GAMEDRIVER_INTERFACE;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_FH_Status(void)
{
	// SLUS_204.64
	return (PS2_MEM_ReadWord(0x90708) == 0x534C5553U && 
			PS2_MEM_ReadWord(0x9070C) == 0x5F323034U &&
			PS2_MEM_ReadWord(0x90710) == 0x2E36343BU);
}

static void PS2_FH_Inject(void)
{
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;

	float looksensitivity = (float)sensitivity / 60.f;


	float camX = PS2_MEM_ReadFloat(FH_CAMX);
	camX += (float)xmouse * looksensitivity;
	while (camX > 360.f)
			camX -= 360;
	while (camX < 0.f)
			camX += 360;
	PS2_MEM_WriteFloat(FH_CAMX, (float)camX);

	float camY = PS2_MEM_ReadFloat(FH_CAMY);
	camY -= (float)(invertpitch ? -ymouse : ymouse) * looksensitivity;
	camY = ClampFloat(camY, -83, 85);
	PS2_MEM_WriteFloat(FH_CAMY, (float)camY);

}