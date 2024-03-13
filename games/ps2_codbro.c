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

#define BRO_CAMY 0x39A748
#define BRO_CAMX 0x39A74C

#define BRO_IS_PAUSED 0x3936D4

static uint8_t PS2_CODBRO_Status(void);
static void PS2_CODBRO_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Call of Duty 2: Big Red One",
	PS2_CODBRO_Status,
	PS2_CODBRO_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_BIGREDONE = &GAMEDRIVER_INTERFACE;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_CODBRO_Status(void)
{
	//SLUS_213.18
	return (PS2_MEM_ReadWord(0x393C78) == 0x534C5553 &&
			PS2_MEM_ReadWord(0x393C7C) == 0x5F323133 &&
			PS2_MEM_ReadWord(0x393C80) == 0x2E31383B);	
}

static void PS2_CODBRO_Inject(void)
{
	
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
		
	if (PS2_MEM_ReadUInt(BRO_IS_PAUSED) == 0x1)
		return;
		
	float looksensitivity = (float)sensitivity;
	float scale = 200.f;
		
	float camX = PS2_MEM_ReadFloat(BRO_CAMX);
	float camY = PS2_MEM_ReadFloat(BRO_CAMY);
	
	
	camX += (float)-xmouse * looksensitivity / scale;
	camY += (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale;
	
	PS2_MEM_WriteFloat(BRO_CAMX, camX);
	PS2_MEM_WriteFloat(BRO_CAMY, camY);
}