#include "stdafx.h"
#include "CRC32.h"

CRC32::CRC32()
{
	Init_CRC32_Table();
}

CRC32::~CRC32()
{

}

void CRC32::Init_CRC32_Table()
{
	// This is the official polynomial used by CRC-32 
	// in PKZip, WinZip and Ethernet. 
	ULONG ulPolynomial = 0x04c11db7;

	// 256 values representing ASCII character codes.
	for(int i = 0; i <= 0xFF; i++)
	{
		crc32_table[i]=Reflect(i, 8) << 24;
		for (int j = 0; j < 8; j++)
			crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
		crc32_table[i] = Reflect(crc32_table[i], 32);
	}
}

ULONG CRC32::Reflect( ULONG ref, char ch )
{
	ULONG value(0);

	// Swap bit 0 for bit 7
	// bit 1 for bit 6, etc.
	for(int i = 1; i < (ch + 1); i++)
	{
		if(ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}

int CRC32::Get_CRC( unsigned char *buffer, DWORD dwSize )
{
	// Be sure to use unsigned variables,
	// because negative values introduce high bits
	// where zero bits are required.
	ULONG  crc(0xffffffff);
	int len;

	len = dwSize;

	// Perform the algorithm on each character
	// in the string, using the lookup table values.
	while(len--)
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];
	// Exclusive OR the result with the beginning value.
	return crc^0xffffffff;
}
