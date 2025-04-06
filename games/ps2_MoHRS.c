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

#define MOHRS_CAMBASEPOINTER 0x35E260

#define MOHRS_CAMY 0x278
#define MOHRS_CAMX 0x274

#define MOHRS_TURRET_CHECK 0xD46

#define MOHRS_CAMY_TURRET 0xE20
#define MOHRS_CAMX_TURRET 0xE1C

#define MOHRS_FOV 0xE30


#define MOHRS_IS_PAUSED 0x359F44





static uint8_t PS2_MOHRS_Status(void);
static void PS2_MOHRS_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Medal of Honor: Rising Sun",
	PS2_MOHRS_Status,
	PS2_MOHRS_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_MOHRS = &GAMEDRIVER_INTERFACE;

static uint32_t camBase = 0;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_MOHRS_Status(void)
{
	  //SLUS_207.53
	return (PS2_MEM_ReadWord(0x93390) == 0x534C5553 &&
			PS2_MEM_ReadWord(0x93394) == 0x5F323037 &&
			PS2_MEM_ReadWord(0x93398) == 0x2E35333B);
}

static void PS2_MOHRS_Inject(void)
{
	
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
	if (PS2_MEM_ReadUInt(MOHRS_IS_PAUSED) == 0x1)
		return;
	
	camBase = PS2_MEM_ReadPointer(MOHRS_CAMBASEPOINTER);
	if (!camBase)
		return;
	
	float looksensitivity = (float)sensitivity;
	float scale = 10000.f;
	
	float camX = PS2_MEM_ReadFloat(camBase + MOHRS_CAMX);
	float camY = PS2_MEM_ReadFloat(camBase + MOHRS_CAMY);

	float camX_Turret = PS2_MEM_ReadFloat(camBase + MOHRS_CAMX_TURRET);
	float camY_Turret = PS2_MEM_ReadFloat(camBase + MOHRS_CAMY_TURRET);

	float fov = PS2_MEM_ReadFloat(camBase + MOHRS_FOV);

	if (PS2_MEM_ReadUInt(camBase + MOHRS_TURRET_CHECK) != 0x1 && PS2_MEM_ReadUInt(camBase + MOHRS_TURRET_CHECK) != 0x9)
	{
		camX += (float)-xmouse * looksensitivity / scale * (fov / 35.f);
		camY += (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale * (fov / 35.f);
		camY = ClampFloat(camY, -1.450000048, 1.450000048);
		while (camX > 3.15)
				camX -= (3.15 * 2.f);
		while (camX < -3.15)
				camX += (3.15 * 2.f);
		PS2_MEM_WriteFloat(camBase + MOHRS_CAMX, camX);
		PS2_MEM_WriteFloat(camBase + MOHRS_CAMY, camY);
	}

	if (PS2_MEM_ReadUInt(camBase + MOHRS_TURRET_CHECK) != 0x0)
	{
		camX_Turret += (float)-xmouse * looksensitivity / scale * (fov / 35.f);
		camY_Turret += (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale * (fov / 35.f);

		if (PS2_MEM_ReadUInt(camBase + MOHRS_TURRET_CHECK) == 0x1) // Normal Turret
		{
			camY_Turret = ClampFloat(camY_Turret, -0.7853981853, 0.7853981853);
			camX_Turret = ClampFloat(camX_Turret, -1.518436551, 1.518436551);
		}

		if (PS2_MEM_ReadUInt(camBase + MOHRS_TURRET_CHECK) == 0x9) // Boat Turret
		{
			camY_Turret = ClampFloat(camY_Turret, -1.04719758, 0);
			while (camX > 3.15)
					camX -= (3.15 * 2.f);
			while (camX < -3.15)
					camX += (3.15 * 2.f);
		}

		PS2_MEM_WriteFloat(camBase + MOHRS_CAMX_TURRET, camX_Turret);
		PS2_MEM_WriteFloat(camBase + MOHRS_CAMY_TURRET, camY_Turret);
	}
	
}