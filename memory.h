//==========================================================================
// Mouse Injector for Dolphin
//==========================================================================
// Copyright (C) 2018 Carnivorous
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the <organization> nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL CARNIVOROUS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==========================================================================
#define NOTWITHINMEMRANGE(X) (X < 0x80000000 || X > 0x81800000) // if X is not within GC memory range
#define WITHINMEMRANGE(X) (!NOTWITHINMEMRANGE(X)) // if X is within GC memory range

extern uint8_t MEM_Init(void);
extern void MEM_Quit(void);
extern void MEM_UpdateEmuoffset(void);
extern int32_t MEM_ReadInt(const uint32_t addr);
extern float MEM_ReadFloat(const uint32_t addr);
extern void MEM_WriteInt(const uint32_t addr, uint32_t value);
extern void MEM_WriteFloat(const uint32_t addr, float value);