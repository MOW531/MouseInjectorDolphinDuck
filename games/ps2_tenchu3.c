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

#define TENCHU3_CAMBASEPOINTER 0x4F81C4
#define TENCHU3_MODELBASEPOINTER 0x4F7928
#define TENCHU3_LEVEL 0x04F4C98


	 #define TENCHU3_MODEL 0x124
	 #define TENCHU3_IS_SNEAKM 0x04F8478
	
	
	 #define TENCHU3_IS_BLOCK 0x04F8478
	
	
	 #define TENCHU3_CAMX 0x274
	 #define TENCHU3_CAMY 0x184

	 #define TENCHU3_MCAMY 0x1E0
	 #define TENCHU3_MCAMX 0x1E4

#define TENCHU3_IS_PAUSED 0x4F8044

//Sneak keys
	// Rikimaru
		#define TENCHU3_IS_SNEAKR1 0x1395767
		#define TENCHU3_IS_SNEAKR2 0x084ADAC

	// Ayame
		#define TENCHU3_IS_SNEAKA1 0x12596C6

static uint8_t PS2_TENCHU3_Status(void);
static void PS2_TENCHU3_Inject(void);

static const GAMEDRIVER GAMEDRIVER_INTERFACE =
{
	"Tenchu: Wrath of Heaven",
	PS2_TENCHU3_Status,
	PS2_TENCHU3_Inject,
	1, // 1000 Hz tickrate
	0 // crosshair sway not supported for driver
};

const GAMEDRIVER *GAME_PS2_TENCHU3 = &GAMEDRIVER_INTERFACE;

static uint32_t camBase = 0;
static uint32_t modelBase = 0;

//==========================================================================
// Purpose: return 1 if game is detected
//==========================================================================
static uint8_t PS2_TENCHU3_Status(void)
{
	// Check if it's the correct game
	//	SLUS_203.97
	return (PS2_MEM_ReadWord(0x93390) == 0x534C5553 &&
			PS2_MEM_ReadWord(0x93394) == 0x5F323033 &&
			PS2_MEM_ReadWord(0x93398) == 0x2E39373B);
}

static void PS2_TENCHU3_Inject(void)
{
	if(xmouse == 0 && ymouse == 0) // if mouse is idle
		return;
	
	// If the games paused
	if (PS2_MEM_ReadUInt(TENCHU3_IS_PAUSED) == 0x1)
		return;
	
	camBase = PS2_MEM_ReadPointer(TENCHU3_CAMBASEPOINTER);
	if (!camBase)
		return;
	
	modelBase = PS2_MEM_ReadPointer(TENCHU3_MODELBASEPOINTER);
	if (!modelBase)
		return;
	
	float looksensitivity = (float)sensitivity;
	float scale = 10000.f;
	float scaleY = 30.f;
	

		// Turns the player
		float Model = PS2_MEM_ReadFloat(modelBase + TENCHU3_MODEL);
		while (Model > 3.120688677f)
			Model -= (3.120688677f * 2.f);
		while (Model < -3.120688677f)
			Model += (3.120688677f * 2.f);
		
		// Normal camera
		float camY = PS2_MEM_ReadFloat(camBase + TENCHU3_CAMY);
		float camX = PS2_MEM_ReadFloat(camBase + TENCHU3_CAMX);
			while (camX > 3.140073538f)
				camX -= (3.140073538f * 2.f);
			while (camX < -3.140073538f)
				camX += (3.140073538f * 2.f);
		
		// Manual camera
		float McamY = PS2_MEM_ReadFloat(camBase + TENCHU3_MCAMY);
		float McamX = PS2_MEM_ReadFloat(camBase + TENCHU3_MCAMX);


		// Turns the player
		 Model += (float)xmouse * looksensitivity / scale;
		
		// Normal camera
		camX += (float)xmouse * looksensitivity / scale;
		camY += (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scaleY;
	
		// Manual camera
		McamX += (float)xmouse * looksensitivity / scale;
		McamY += (float)(invertpitch ? -ymouse : ymouse) * looksensitivity / scale;
		
		float Sneaking = 0;
		
		// Rikimaru
		if ((float)PS2_MEM_ReadUInt8(TENCHU3_IS_SNEAKR1) != 63.f && PS2_MEM_ReadWord(TENCHU3_LEVEL) == 0x50CD6800 && PS2_MEM_ReadWord(TENCHU3_LEVEL + 0x4) == 0x50CD6900 || PS2_MEM_ReadUInt8(TENCHU3_IS_SNEAKR2) == 0xE0 && PS2_MEM_ReadWord(TENCHU3_LEVEL) == 0xD0D36800 && PS2_MEM_ReadWord(TENCHU3_LEVEL + 0x4) == 0xD0D36900) {
			Sneaking = 1;
		}
		
		// Ayame
		else if ((float)PS2_MEM_ReadUInt8(TENCHU3_IS_SNEAKA1) == 32.f && PS2_MEM_ReadWord(TENCHU3_LEVEL) == 0x50D26A00 && PS2_MEM_ReadWord(TENCHU3_LEVEL + 0x4) == 0x50D26B00) {
			Sneaking = 1; }
			
		else {
				Sneaking = 0;
			}


		// Check if the player should be allowed to turn
		 if (Sneaking == 0 && (float)PS2_MEM_ReadUInt8(TENCHU3_IS_BLOCK) != 32.f && (float)PS2_MEM_ReadUInt8(TENCHU3_IS_BLOCK) != 36.f && (float)PS2_MEM_ReadUInt8(TENCHU3_IS_BLOCK) != 4.f && (float)PS2_MEM_ReadUInt8(TENCHU3_IS_BLOCK) != 16.f && (float)PS2_MEM_ReadUInt8(TENCHU3_IS_SNEAKM) != 12.f && (float)PS2_MEM_ReadUInt8(TENCHU3_IS_SNEAKM) != 0.f && (float)PS2_MEM_ReadUInt8(TENCHU3_IS_SNEAKM) != 64.f)
			 PS2_MEM_WriteFloat(modelBase + TENCHU3_MODEL, Model);
		
		// Writes the normal camera
		PS2_MEM_WriteFloat(camBase + TENCHU3_CAMX, camX);
		//PS2_MEM_WriteFloat(camBase + TENCHU3_CAMY, camY);
	
		// Writes the manual camera
		PS2_MEM_WriteFloat(camBase + TENCHU3_MCAMX, McamX);
		PS2_MEM_WriteFloat(camBase + TENCHU3_MCAMY, McamY);
}