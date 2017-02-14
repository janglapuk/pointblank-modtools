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

// I3IToolDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <Shlwapi.h>
#include "Hyperlink.h"

using namespace std;

struct DDS_PIXELFORMAT {
	DWORD dwSize;
	DWORD dwFlags;
	union {
		char szFourCC[4];
		DWORD dwFourCC;
	};
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwABitMask;
};

typedef struct {
	union {
		char		szHeader[4];
		DWORD		dwHeader;
	};
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwHeight;
	DWORD           dwWidth;
	DWORD           dwPitchOrLinearSize;
	DWORD           dwDepth;
	DWORD           dwMipMapCount;
	DWORD           dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	DWORD           dwCaps;
	DWORD           dwCaps2;
	DWORD           dwCaps3;
	DWORD           dwCaps4;
	DWORD           dwReserved2;
} DDS_HEADER;

#pragma pack(push, 1)
typedef struct {
	ULONG ulMagic;
	USHORT usUnknown1;
	USHORT usWidth;
	USHORT usHeight;
	ULONG ulFormat;
	BYTE Padding1[10];
	USHORT usMipsLevel;
	USHORT usMarkerLen;
	BYTE Padding2[32];
} I3I_HEADER;
#pragma pack(pop)

typedef struct _I3IFileList {
	enum {
		Invalid,
		ToI3I,
		ToDDS
	};

	CString csFullPath;
	CString csFilename;
	CString csExt;
	
	CString csSavedFilename;

	int nDirection;
	ULONG ulHeight;
	ULONG ulWidth;
	union {
		ULONG ulFormat;
		char szFormat[4];
	};
	BYTE ucNull;

	ULONG ulMipMapCount;
	ULONG ulMarketLen;
	CString csMarker;

	_I3IFileList(CString fp, ULONG h, ULONG w, ULONG fmt, ULONG mmc, ULONG mark, CString markstr="") {
		csFullPath	= fp;
		csFilename	= PathFindFileName(csFullPath);
		csExt		= PathFindExtension(csFilename);
		csExt.MakeLower();

		if(csExt == ".i3i")
			nDirection = ToDDS;
		else if(csExt == ".dds")
			nDirection = ToI3I;
		else
			nDirection = Invalid;

		ulHeight	= h;
		ulWidth		= w;
		ulFormat	= fmt;

		ucNull		= 0;
		
		ulMipMapCount	= mmc;

		ulMarketLen	= mark;
		csMarker	= markstr;

	}

} I3IFileList;

// CI3IToolDlg dialog
class CI3IToolDlg : public CDialog
{
// Construction
public:
	CI3IToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_I3ITOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	enum DDS_PF {
		DDS_PF_DXT1,
		DDS_PF_DXT1_ALPHA,
		DDS_PF_DXT3,
		DDS_PF_DXT5_INTERP
	};

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam);
	void ProcessFile();
	void InsertToVector(CString csFile);
	void FillComboList();
	BOOL IsFileExists(CString csFile);

public:
	CFont m_Font;
	CStatic m_stcDropHere;
	CComboBox m_cboFiles;
	vector<I3IFileList> m_vFileList;

	char m_szFormat[MAX_PATH * 2];
	afx_msg void OnCbnSelchangeCombo1();
	void UpdateInfo();
	CString EnumerateFormat(ULONG ulFmt);
	DDS_HEADER *GenerateDDSHeader(ULONG pf, DWORD dwHeight, DWORD dwWidth, DWORD dwMipMapCount=0);
	afx_msg void OnBnClickedBtnConvsingle();

	CMyHyperLink m_linkAuthor;
};
