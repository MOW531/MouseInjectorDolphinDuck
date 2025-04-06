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

#define MOHFL_CAMBASEPOINTER 0x23F4A8

#define MOHFL_CAMY 0x2A8
#define MOHFL_CAMX 0x2A4

#define MOHFL_TURRET_CHECK 0x61C  // 0x648

#define MOHFL_CAMY_TURRET 0x430
#define MOHFL_CAMX_TURRET 0x42C


#define MOHFL_FOV 0x43C


#define MOHFL_IS_PAUSED 0x23FA04





static uint8_t PS2_MOHFL_Status(void);
static void PS2_MOHFL_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Medal of Honor: Frontline",
	PS2_MOHFL_Status,
	PS2_MOHFL_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_MOHFL = &GAMEDRIVER_INTERFACE;

static uint32_t camBase = 0;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_MOHFL_Status(void)
{
	//SLUS_203.68
	return (PS2_MEM_ReadWord(0x93390) == 0x534C5553 &&
			PS2_MEM_ReadWord(0x93394) == 0x5F323033 &&
			PS2_MEM_ReadWord(0x93398) == 0x2E36383B);
}

static void PS2_MOHFL_Inject(void)
{
	
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
	if (PS2_MEM_ReadUInt(MOHFL_IS_PAUSED) == 0x1)
		return;
	
	camBase = PS2_MEM_ReadPointer(MOHFL_CAMBASEPOINTER);
	if (!camBase)
		return;
	
	float looksensitivity = (float)sensitivity;
	float scale = 10000.f;
	
	float camX = PS2_MEM_ReadFloat(camBase + MOHFL_CAMX);
	float camY = PS2_MEM_ReadFloat(camBase + MOHFL_CAMY);

	float camX_Turret = PS2_MEM_ReadFloat(camBase + MOHFL_CAMX_TURRET);
	float camY_Turret = PS2_MEM_ReadFloat(camBase + MOHFL_CAMY_TURRET);


	float fov = PS2_MEM_ReadFloat(camBase + MOHFL_FOV);

	if (PS2_MEM_ReadUInt(camBase + MOHFL_TURRET_CHECK) != 0x15)
	{
		camX += (float)-xmouse * looksensitivity / scale * (fov / 35.f);
		camY += (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale * (fov / 35.f);
		camY = ClampFloat(camY, -1.450000048, 1.450000048);
		while (camX > 6.28318529129)
				camX -= (6.28318529129 * 2.f);
		while (camX < -6.28318529129)
				camX += (6.28318529129 * 2.f);
		PS2_MEM_WriteFloat(camBase + MOHFL_CAMX, camX);
		PS2_MEM_WriteFloat(camBase + MOHFL_CAMY, camY);
	}
	

	if (PS2_MEM_ReadUInt(camBase + MOHFL_TURRET_CHECK) == 0x15)
	{
		camX_Turret += (float)-xmouse * looksensitivity / scale * (fov / 35.f);
		camY_Turret += (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale * (fov / 35.f);
		camY_Turret = ClampFloat(camY_Turret, -0.5585054159, 0.5585054159);
		while (camX_Turret > 6.28318529129)
			camX_Turret -= (6.28318529129 * 2.f);
		while (camX_Turret < -6.28318529129)
			camX_Turret += (6.28318529129 * 2.f);
		PS2_MEM_WriteFloat(camBase + MOHFL_CAMX_TURRET, camX_Turret);
		PS2_MEM_WriteFloat(camBase + MOHFL_CAMY_TURRET, camY_Turret);
	}
}