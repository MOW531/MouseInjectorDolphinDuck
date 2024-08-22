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
#include <stdio.h>
#include "../main.h"
#include "../memory.h"
#include "../mouse.h"
#include "game.h"

//0x806FEA10 LOCK THIS! 

#define ARMORINES_CAMY 0x806FE98C
#define ARMORINES_CAMY_ACC 0x806FE9A0 //806FEA10
#define ARMORINES_CAMY_ACC2 0x806FF0AC

#define ARMORINES_CAMY_VEH 0x806FEA14
#define ARMORINES_CAMY_VEH2 0x806FF0B0

#define ARMORINES_VEH_CHECK 0x800D77CC

#define ARMORINES_CAMX 0x806FE39C

static uint8_t N64_ARMORINES_Status(void);
static uint8_t N64_ARMORINES_DetectCam(void);
static void N64_ARMORINES_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Armorines: Project S.W.A.R.M.",
	N64_ARMORINES_Status,
	N64_ARMORINES_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_N64_ARMORINES = &GAMEDRIVER_INTERFACE;

static uint8_t N64_ARMORINES_Status(void)
{
	return (N64_MEM_ReadUInt(0x80000000) == 0x3C1A002A && N64_MEM_ReadUInt(0x80000004) == 0x275A18B0); // unique header in RDRAM
}

static void N64_ARMORINES_Inject(void)
{
	//if(xmouse == 0 && ymouse == 0) // if mouse is idle
	//	return;

	const float looksensitivity = (float)sensitivity;
	const float scale = 6400.f;


	float camX = N64_MEM_ReadFloat(ARMORINES_CAMX);
	float camY = N64_MEM_ReadFloat(ARMORINES_CAMY);
	float camY_acc = N64_MEM_ReadFloat(ARMORINES_CAMY_ACC);
	float camY_acc2 = N64_MEM_ReadFloat(ARMORINES_CAMY_ACC2);

	float camY_veh = N64_MEM_ReadFloat(ARMORINES_CAMY_VEH);
	float camY_veh2 = N64_MEM_ReadFloat(ARMORINES_CAMY_VEH2);

	camX += (float)xmouse * looksensitivity / scale;
	camY += (float)(float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale;
	camY_acc -= (float)(float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale;
	camY_acc2 -= (float)(float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale;

	camY_veh += (float)xmouse * looksensitivity / scale;
	camY_veh2 += (float)xmouse * looksensitivity / scale;
	
	camY = ClampFloat(camY, -1.f, 1.f);
	camY_acc = ClampFloat(camY_acc, -1.570796013, 1.570796013);
	camY_acc2 = ClampFloat(camY_acc2, -1.570796013, 1.570796013);






	N64_MEM_WriteFloat(ARMORINES_CAMY, camY);
	
	if (N64_MEM_ReadUInt(0x806FE420) == 0x13140100) {

		N64_MEM_WriteFloat(ARMORINES_CAMY_VEH, camY_veh);
		N64_MEM_WriteFloat(ARMORINES_CAMY_VEH2, camY_veh2);
		
	} else {
		N64_MEM_WriteFloat(ARMORINES_CAMX, camX);
	}
	//N64_MEM_WriteFloat(ARMORINES_CAMY_ACC, camY);
	//N64_MEM_WriteFloat(ARMORINES_CAMY_ACC2, camY_acc2);
	//N64_MEM_WriteFloat(0x806FE9A0, camY);
	 
}