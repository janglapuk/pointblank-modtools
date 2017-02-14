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


#ifndef Structures_h__
#define Structures_h__

#define NODE_PADDING	0x4C

class CPackHeader
{
public:
	char szHeaderID[8]; //0x0000 
	DWORD N00CE995A; //0x0008 
	DWORD N00CE995B; //0x000C 
	DWORD dwStringTableOff; //0x0010 
	char _0x0014[4];
	DWORD dwStringTableSize; //0x0018 
	char _0x001C[4];
	DWORD dwNodeCount; //0x0020 
	DWORD dwDirTableOff; //0x0024 
	char _0x0028[4];
	DWORD dwDirTableSize; //0x002C 
	char _0x0030[4];
	DWORD dwNodeSize; //0x0034 
	char _0x0038[4];
	char N00D72CC0[16]; //0x003C 
	char _0x004C[108];

};//Size=0x00B8

// class CPackFileInfo
// {
// public:
// 	char szFilename[32]; //0x0000 
// 	char _0x0020[20];
// 	char N0193C181[2]; //0x0034 
// 	WORD SizeOr_1; //0x0036 52
// 	WORD OffsShift_1; //0x0038 54
// 	WORD SizeShift_1; //0x003A 56
// 	char _0x003C[4];
// 	WORD OffsOr_1; //0x0040 5c
// 	char N01970D81[4]; //0x0042 
// 	WORD SizeOr_2; //0x0046 52
// 	WORD OffsShift_2; //0x0048 54
// 	WORD SizeShift_2; //0x004A 56
// 	char N01A26723[4]; //0x004C 
// 	WORD OffsOr_2; //0x0050 5c
// 	char _0x0052[8];
// 	char N019E321E[2]; //0x005A 
// 	BYTE Ended; //0x005C  if Ended==0, use _1, otherwise _2
// 
// };//Size=0x005D

class CPackFileInfo
{
public:
	char szFilename[32]; //0x0000 
	char _0x0020[20];
	char N0193C181[2]; //0x0034 
	WORD SizeOr_1; //0x0036 52
	WORD OffsShift_1; //0x0038 54
	WORD SizeShift_1; //0x003A 56
	char _0x003C[4];
	WORD OffsOr_1; //0x0040 5c
	char N01970D81[4]; //0x0042 
	WORD SizeOr_2; //0x0046 52
	WORD OffsShift_2; //0x0048 54
	WORD SizeShift_2; //0x004A 56
	char N01A26723[4]; //0x004C 
	WORD OffsOr_2; //0x0050 5c
	char N019E3220[3]; //0x0052 
	DWORD Ended; //0x0055
};//Size=0x005D

class CNodeInfo
{
public:
	char _0x0000[4];
	DWORD dwIndex; //0x0004 
	char _0x0008[4];
	DWORD dwOffset; //0x000C 
	char _0x0010[4];
	DWORD dwSize; //0x0014 
	char _0x0018[4];
};//Size=0x001C

class CHeaderNodeInfo {
public:
	ULONG BaseAddr;
	ULONG Index;
	ULONG Offset;
	ULONG Size;
	
	CHeaderNodeInfo(ULONG ba, ULONG idx, ULONG off, ULONG sz) {
		BaseAddr = ba;
		Index = idx;
		Offset = off;
		Size = sz;
	}
};

class CNodeFileInfo {
public:
	CString Filename;
	ULONG Offset;
	ULONG Size;
	ULONG RawOffset;
	bool Padded;

	CNodeFileInfo(CString fn, ULONG off, ULONG sz) {
		Filename = fn;
		Offset = off;
		Size = sz;
	}

	CNodeFileInfo(CString fn, ULONG off, ULONG sz, ULONG ro, bool pad) {
		Filename = fn;
		Offset = off;
		Size = sz;
		RawOffset = ro;
		Padded = pad;
	}
};

// class CSingleNode {
// public:
// 	CString NodeName;
// 	ULONG Index;
// 	ULONG Offset;
// 	ULONG Size;
// 	ULONG FileCount;
// 
// 	std::vector<CNodeFileInfo> Files;
// 
// 	CSingleNode(CString nn, ULONG idx, ULONG off, ULONG sz, ULONG fc) {
// 		Index = idx;
// 		NodeName = nn;
// 		Offset = off;
// 		Size = sz;
// 		FileCount = fc;
// 	}
// 
// };

class CSingleNode {
public:
	CString NodeName;
	ULONG Index;
	ULONG Offset;
	ULONG Size;
	ULONG FileCount;
	ULONG DirTableOffs;

	// tree node info
	std::vector<int> ChildId;

	// file info
	std::vector<CNodeFileInfo> Files;

	CSingleNode(CString nn, ULONG idx, ULONG off, ULONG sz, ULONG fc) {
		Index = idx;
		NodeName = nn;
		Offset = off;
		Size = sz;
		FileCount = fc;
	}

	bool HasChild() const {
		return !ChildId.empty();
	}

	bool IsLeaf() const {
		return ChildId.empty();
	}

	bool IsRoot() const {
		return NodeName == "/";
	}

};

typedef struct {
	ULONG Unknown;
	ULONG FileCount;
} sNodeFileInfo;


#endif // Structures_h__
