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


#ifndef Ogg_h__
#define Ogg_h__

#pragma pack(push, 1)
class OggSegment
{
public:
	BYTE PacketID; //0x0000 
	char Signature[6]; //0x0001 
	DWORD VorbisVersion; //0x0007 
	BYTE Channels; //0x000B 
	DWORD SampleRate; //0x000C 
	DWORD BitrateMax; //0x0010 
	DWORD BitrateNom; //0x0014 
	DWORD BitrateMin; //0x0018 
	BYTE blocksize_0; //0x001C 
	BYTE framing_flag; //0x001D 

};//Size=0x001E

class OggHeader
{
public:
	DWORD Signature; //0x0000 
	BYTE Version; //0x0004 
	BYTE HeaderType; //0x0005 
	__int64 GranulePosition; //0x0006 
	DWORD BitstreamSerialNumber; //0x000E 
	DWORD PageSequenceNumber; //0x0012 
	DWORD CRCChecksum; //0x0016 
	BYTE SegmentsCount; //0x001A 
	BYTE LacingSegments; //0x001B 
	OggSegment Segments[1]; //0x001C 

};//Size=0x003A
#pragma pack(pop)

#endif // Ogg_h__
