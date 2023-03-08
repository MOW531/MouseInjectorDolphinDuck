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

#define C12_TP_CAMY 0x1DE800
#define C12_TP_CAMX 0x1DE802

static uint8_t PS1_C12_Status(void);
static void PS1_C12_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"C-12: Final Resistance",
	PS1_C12_Status,
	PS1_C12_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway supported for driver
};

const GAMEDRIVER *GAME_PS1_C12FINALRESISTANCE = &GAMEDRIVER_INTERFACE;

static float xAccumulator = 0.f;
static float yAccumulator = 0.f;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS1_C12_Status(void)
{
	return (PS1_MEM_ReadWord(0x9274) == 0x53435553U && PS1_MEM_ReadWord(0x9278) == 0x5F393436U && PS1_MEM_ReadWord(0x927C) == 0x2E36363BU);
}
//==========================================================================
// Purpose: calculate mouse look and inject into current game
//==========================================================================
static void PS1_C12_Inject(void)
{
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
	
	uint16_t camX = PS1_MEM_ReadHalfword(C12_TP_CAMX);
	uint16_t camY = PS1_MEM_ReadHalfword(C12_TP_CAMY);
	float camXF = (float)camX;
	float camYF = (float)camY;

	const float looksensitivity = (float)sensitivity / 20.f;
	const float scale = 1.f;

	float dx = (float)xmouse * looksensitivity * scale;
	AccumulateAddRemainder(&camXF, &xAccumulator, xmouse, dx);

	float ym = (float)(invertpitch ? -ymouse : ymouse);
	float dy = -ym * looksensitivity * scale;
	AccumulateAddRemainder(&camYF, &yAccumulator, ym, dy);

	// clamp y-axis
	// if (camYF > 600 && camYF < 32000)
	// 	camYF = 600;
	// if (camYF < 65000 && camYF > 32000)
	// 	camYF = 65000;

	PS1_MEM_WriteHalfword(C12_TP_CAMX, (uint16_t)camXF);
	PS1_MEM_WriteHalfword(C12_TP_CAMY, (uint16_t)camYF);
}