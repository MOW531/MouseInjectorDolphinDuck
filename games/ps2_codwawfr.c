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

#define TAU 6.2831853f 
#define PI 3.14159265f

#define WAWFR_CAMY 0x1EA9CC0
#define WAWFR_CAMX 0x1EA9CC4

#define WAWFR_IS_PAUSED 0x04D8F38

#define WAWFR_FOV 0x483004


static uint8_t PS2_CODWAWFR_Status(void);
static void PS2_CODWAWFR_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Call of Duty World At War: Final Fronts",
	PS2_CODWAWFR_Status,
	PS2_CODWAWFR_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_WORLDATWARFINALFRONTS = &GAMEDRIVER_INTERFACE;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_CODWAWFR_Status(void)
{
	//SLUS-21746
	return (PS2_MEM_ReadWord(0x4D2B22) == 0x534C5553);	
}

static void PS2_CODWAWFR_Inject(void)
{
	
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
		
	if (PS2_MEM_ReadUInt(WAWFR_IS_PAUSED) == 0x1)
		return;
		
	float looksensitivity = (float)sensitivity;
	float fov = PS2_MEM_ReadFloat(WAWFR_FOV);
	float scale = 200.f;
	if (fov > 1.0f) {
		scale = 200.f * fov;}
	else {
		scale = 200.f;}
		
	float camX = PS2_MEM_ReadFloat(WAWFR_CAMX);
	float camY = PS2_MEM_ReadFloat(WAWFR_CAMY);
	
	    camX = camX * (180.f / PI);
    camY = camY * (180.f / PI);
	
	camX -= ((float)-xmouse  / scale) * looksensitivity ;
	camY -= ((float)(invertpitch ? -ymouse : ymouse) / scale) * looksensitivity ;
	camY = ClampFloat(camY, -1.299999952f * (180.f / PI), 1.299999952f * (180.f / PI));

	
	camX = camX * (PI / 180.f);
    camY = camY * (PI / 180.f);
	
	PS2_MEM_WriteFloat(WAWFR_CAMX, camX);
	PS2_MEM_WriteFloat(WAWFR_CAMY, camY);
}