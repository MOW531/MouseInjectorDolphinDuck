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

#define MOHEA_CAMBASEPOINTER 0x1FD7400

#define MOHEA_CAMY 0x4C8
#define MOHEA_CAMX 0x4C0

#define MOHEA_FOV 0x498

#define MOHEA_IS_PAUSED 0x4981D8


static uint8_t PS2_MOHEA_Status(void);
static void PS2_MOHEA_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Medal of Honor: European Assault",
	PS2_MOHEA_Status,
	PS2_MOHEA_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_MOHEA = &GAMEDRIVER_INTERFACE;


static uint32_t camBase = 0;
static uint32_t fovBase = 0;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_MOHEA_Status(void)
{
	  //SLUS_211.99
	return (PS2_MEM_ReadWord(0x93390) == 0x534C5553 &&
			PS2_MEM_ReadWord(0x93394) == 0x5F323131 &&
			PS2_MEM_ReadWord(0x93398) == 0x2E39393B);
}

static void PS2_MOHEA_Inject(void)
{
	fovBase = PS2_MEM_ReadPointer(MOHEA_CAMBASEPOINTER);
	 if (!fovBase)
		 return;
	
	 // float fov = 40.f;
		
	// if (PS2_MEM_ReadFloat(fovBase - MOHEA_FOV) == 35.f) {
		 // PS2_MEM_WriteFloat(fovBase - MOHEA_FOV, fov);
	// }
	
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
	if (PS2_MEM_ReadUInt(MOHEA_IS_PAUSED) == 0x1)
		return;
	
	camBase = PS2_MEM_ReadPointer(MOHEA_CAMBASEPOINTER);
	if (!camBase)
		return;
	
	float looksensitivity = (float)sensitivity;
	float scale = 10000.f;
		if (PS2_MEM_ReadFloat(fovBase - MOHEA_FOV) <= 10.f) {
		scale = 80000.f;}
	else if (PS2_MEM_ReadFloat(fovBase - MOHEA_FOV) <= 15.f){
		scale = 60000.f;
	}
	else if (PS2_MEM_ReadFloat(fovBase - MOHEA_FOV) <= 25.f){
		scale = 20000.f;
	}
	else {
		scale = 10000.f;}
	
	float camX = PS2_MEM_ReadFloat(camBase - MOHEA_CAMX);
	float camY = PS2_MEM_ReadFloat(camBase - MOHEA_CAMY);
	
	camX += (float)-xmouse * looksensitivity / scale;
	camY += (float)(invertpitch ? ymouse : -ymouse) * looksensitivity / scale;
	camY = ClampFloat(camY, -1.483529925, 1.483529925);
	while (camX > 6.28318529129)
			camX -= (6.28318529129 * 2.f);
	while (camX < -6.28318529129)
			camX += (6.28318529129 * 2.f);
	
	PS2_MEM_WriteFloat(camBase - MOHEA_CAMX, camX);
	PS2_MEM_WriteFloat(camBase - MOHEA_CAMY, camY);
}