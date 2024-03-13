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

#define BFTP_CAMY 0x1A51B18
#define BFTP_CAMX 0x1A51B1C

#define BFTP_CAMY2 0x1A51AB4


//#define BFTP_ARM1 0x1A518F8
//#define BFTP_ARM2 0x1A51910


#define BFTP_IS_PAUSED 0x0760B5C

static uint8_t PS2_BFTP_Status(void);
static void PS2_BFTP_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"The History Channel: The Battle for the Pacific",
	PS2_BFTP_Status,
	PS2_BFTP_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_BATTLEPACIFIC = &GAMEDRIVER_INTERFACE;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_BFTP_Status(void)
{
	  //SLUS_217.12
	return (PS2_MEM_ReadWord(0x93390) == 0x534C5553 &&
			PS2_MEM_ReadWord(0x93394) == 0x5F323137 &&
			PS2_MEM_ReadWord(0x93398) == 0x2E31323B);
}

static void PS2_BFTP_Inject(void)
{
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
	
	if (PS2_MEM_ReadUInt(BFTP_IS_PAUSED) == 0x1)
		return;
	
	float looksensitivity = (float)sensitivity;
	float scale = 200.f;
	
	float camX = PS2_MEM_ReadFloat(BFTP_CAMX);
	float camY = PS2_MEM_ReadFloat(BFTP_CAMY);
	
	camX += (float)xmouse * looksensitivity / scale;
	camY += (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale;
	camY = ClampFloat(camY, -90, 90);
	
	PS2_MEM_WriteFloat(BFTP_CAMX, camX);
	//PS2_MEM_WriteFloat(BFTP_ARM1, camX);
	//PS2_MEM_WriteFloat(BFTP_ARM2, camX);
	PS2_MEM_WriteFloat(BFTP_CAMY, camY);
	PS2_MEM_WriteFloat(BFTP_CAMY2, camY);
}