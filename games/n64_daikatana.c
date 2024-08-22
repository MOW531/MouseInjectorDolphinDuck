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

#define DAIKATANA_CAMY 0x8020A92C
#define DAIKATANA_CAMX 0x8020A930

static uint8_t N64_DAIKATANA_Status(void);
static uint8_t N64_DAIKATANA_DetectCam(void);
static void N64_DAIKATANA_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"John Romero's Daikatana",
	N64_DAIKATANA_Status,
	N64_DAIKATANA_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_N64_DAIKATANA = &GAMEDRIVER_INTERFACE;

static uint8_t N64_DAIKATANA_Status(void)
{
	return (N64_MEM_ReadUInt(0x80000000) == 0x3C1A8019 && N64_MEM_ReadUInt(0x80000004) == 0x275A9050); // unique header in RDRAM
}

static void N64_DAIKATANA_Inject(void)
{
	//if(xmouse == 0 && ymouse == 0) // if mouse is idle
	//	return;

	const float looksensitivity = (float)sensitivity;
	const float scale = 64.f;


	float camX = N64_MEM_ReadFloat(DAIKATANA_CAMX);
	float camY = N64_MEM_ReadFloat(DAIKATANA_CAMY);

	camX -= (float)xmouse * looksensitivity / scale;
	camY += (float)(float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale;
	
	camY = ClampFloat(camY, 10.f, 170.f);

	while (camX > 360.f)
			camX -= (360.f * 1.f);
	while (camX < 0.f)
			camX += (360.f * 1.f);





	N64_MEM_WriteFloat(DAIKATANA_CAMY, camY);
	N64_MEM_WriteFloat(DAIKATANA_CAMX, camX);
	 
}