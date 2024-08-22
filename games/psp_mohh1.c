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

#define TAU 6.2831853f // 0x40C90FDB

//#define MOHH1_CAMY 0x196A6B8
//#define MOHH1_CAMX 0x196A6D4

#define MOHH1_CAMBASE_PTR 0xD8361C

#define MOHH1_CAMY 0x188
#define MOHH1_CAMX 0x1A4
#define MOHH1_FOV 0x1E8

static uint8_t PSP_MOHH1_Status(void);
static void PSP_MOHH1_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Medal of Honor: Heros",
	PSP_MOHH1_Status,
	PSP_MOHH1_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway supported for driver
};

const GAMEDRIVER *GAME_PSP_MOHH1 = &GAMEDRIVER_INTERFACE;

static uint32_t camBase = 0;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PSP_MOHH1_Status(void)
{
	return (PSP_MEM_ReadWord(0xD05FC4) == 0x554C5553U &&
			PSP_MEM_ReadWord(0xD05FC8) == 0x31303134U && 
			PSP_MEM_ReadWord(0xD05FCC) == 0x31000000U);
}

//==========================================================================
// Purpose: calculate mouse look and inject into current game
//==========================================================================
static void PSP_MOHH1_Inject(void)
{
	// TODO: look for 2-byte isBusy value

	camBase = PSP_MEM_ReadPointer(MOHH1_CAMBASE_PTR);
	if (!camBase)
		return;

	float fov = PSP_MEM_ReadFloat(camBase + MOHH1_FOV);
	if (fov == 30)
		PSP_MEM_WriteFloat(camBase + MOHH1_FOV, (float)35);

	if (xmouse == 0 && ymouse == 0) // if mouse is idle
		return;

	float camX = PSP_MEM_ReadFloat(camBase + MOHH1_CAMX);
	float camY = PSP_MEM_ReadFloat(camBase + MOHH1_CAMY);

	const float looksensitivity = (float)sensitivity / 20.f;
	const float scale = 20000.f;

	camX -= (float)xmouse * looksensitivity / scale * fov;
	camY -= (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale * fov;

	while (camX > TAU / 2)
		camX -= TAU;
	while (camX < -TAU / 2)
		camX += TAU;
	camY = ClampFloat(camY, -1.483529806, 1.483529806);

	PSP_MEM_WriteFloat(camBase + MOHH1_CAMX, (float)camX);
	PSP_MEM_WriteFloat(camBase + MOHH1_CAMY, (float)camY);
}