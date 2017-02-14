// MIT License
//
// Copyright (c) 2013 Thami Rusdi Agus - abujafar (@janglapuk)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "stdafx.h"
#include "Crypt.h"

typedef void (__cdecl *pBitRotate)(void *, unsigned int, unsigned int);

void __declspec(naked) BRE()
{
	__asm {
		PUSH EBP
			MOV EBP,DWORD PTR SS:[ESP+0x10]
		PUSH ESI
			MOV ESI,DWORD PTR SS:[ESP+0xC]
		MOV AL,BYTE PTR DS:[ESI]
		PUSH EDI
			MOV EDI,DWORD PTR SS:[ESP+0x14]
		MOV ECX,0x8
			MOV BYTE PTR SS:[ESP+0x10],AL
			SUB ECX,EBP
			XOR EAX,EAX
			MOV DWORD PTR SS:[ESP+0x18],ECX
			TEST EDI,EDI
			JLE jmp005
			PUSH EBX
			JMP jmp002
			LEA ESP,DWORD PTR SS:[ESP]
jmp001:
		MOV ECX,DWORD PTR SS:[ESP+0x1C]
jmp002:
		MOV BL,BYTE PTR DS:[EAX+ESI]
		LEA EDX,DWORD PTR DS:[EDI-0x1]
		CMP EAX,EDX
			JGE jmp003
			MOV DL,BYTE PTR DS:[EAX+ESI+0x1]
		JMP jmp004
jmp003:
		MOV DL,BYTE PTR SS:[ESP+0x14]
jmp004:
		SHR DL,CL
			MOV ECX,EBP
			SHL BL,CL
			INC EAX
			OR DL,BL
			CMP EAX,EDI
			MOV BYTE PTR DS:[EAX+ESI-0x1],DL
			JL jmp001
			POP EBX
jmp005:
		POP EDI
			POP ESI
			POP EBP
			RETN
	}
}

void __declspec(naked) BRD() {
	__asm {
		MOV EAX,DWORD PTR SS:[ESP+0x8]		// length
		PUSH EBX
			MOV EBX,DWORD PTR SS:[ESP+0x10]		//shiftkey
		PUSH EBP
			PUSH ESI
			PUSH EDI
			MOV EDI,DWORD PTR SS:[ESP+0x14]		//buffer
		MOV CL,BYTE PTR DS:[EDI+EAX-0x1]
		MOV EBP, 0x8
			LEA ESI,DWORD PTR DS:[EAX-0x1]
		SUB EBP,EBX
			MOV BYTE PTR SS:[ESP+0x1C],CL
			TEST ESI,ESI
			JL jump004
jump001:
		MOV AL,BYTE PTR DS:[ESI+EDI]
		TEST ESI,ESI
			JLE jump002
			MOV DL,BYTE PTR DS:[ESI+EDI-0x1]
		JMP jump003
jump002:
		MOV DL,BYTE PTR SS:[ESP+0x1C]
jump003:
		MOV CL,BL
			SHR AL,CL
			MOV ECX,EBP
			SHL DL,CL
			OR AL,DL
			SUB ESI, 0x1
			MOV BYTE PTR DS:[ESI+EDI+0x1],AL
			JNS jump001
jump004:
		POP EDI
			POP ESI
			POP EBP
			POP EBX
			RETN
	}
}


bool NotXorBit(unsigned char *ucBytes, int nLength, unsigned char ucXor/*=0x63*/) {
	if(!nLength) 
		return false;

	for(int i=0; i<nLength; i++) 
		ucBytes[i] = ~ucBytes[i] ^ ucXor;

	return true;
}

bool NotXorBit(unsigned char *ucBytes, int nLength, CString &csBuf, unsigned char ucXor/*=0x63*/) {
	unsigned char *ucBuff = new unsigned char[nLength];
	if(memcpy_s(ucBuff, nLength, ucBytes, nLength) != 0)
		return false;

	if(NotXorBit(ucBuff, nLength, ucXor) == false)
		return false;

	csBuf = CString((char *)ucBuff);
	return true;
}

pBitRotate __BitRotateEncrypt = (pBitRotate)BRE;
pBitRotate __BitRotateDecrypt = (pBitRotate)BRD;

void __cdecl BitRotateEncrypt( void *pBuffer, unsigned int nSize, unsigned int nKey )
{
	__BitRotateEncrypt(pBuffer, nSize, nKey);
}

void __cdecl BitRotateDecrypt( void *pBuffer, unsigned int nSize, unsigned int nKey )
{
	__BitRotateDecrypt(pBuffer, nSize, nKey);
}
