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

// DDSPatcherDlg.h : header file
//

#pragma once

#include <boost/shared_ptr.hpp>
#include "Hyperlink.h"

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

// CDDSPatcherDlg dialog
class CDDSPatcherDlg : public CDialog
{
// Construction
public:
	CDDSPatcherDlg(CWnd* pParent = NULL);	// standard constructor
	~CDDSPatcherDlg();

// Dialog Data
	enum { IDD = IDD_DDSPATCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	void EnableDisableControl(bool bState);
	BOOL IsFileExists(CString csFilename);

	boost::shared_ptr<BYTE> *m_pDDSBuffer;
	ULONG m_ulDDSBufferSize;

	boost::shared_ptr<BYTE> *m_pI3IBuffer;
	ULONG m_ulI3IBufferSize;
	
	CString m_csOpenedDDSPath;
	CString m_csOpenedI3IPath;

	CMyHyperLink m_linkAuthor;
};
