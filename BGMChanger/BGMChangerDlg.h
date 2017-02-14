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


// BGMChangerDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "CI3Pack.h"

#include "Hyperlink.h"

// CBGMChangerDlg dialog
class CBGMChangerDlg : public CDialog
{
// Construction
public:
	CBGMChangerDlg(CWnd* pParent = NULL);	// standard constructor

	~CBGMChangerDlg();

// Dialog Data
	enum { IDD = IDD_BGMCHANGER_DIALOG };

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
	CButton m_btnBrowse;
	CEdit m_editPath;
	afx_msg void OnBnClickedBtnbrowse();
	CString FormatSize(ULONG ulSize);
	void EmptyStaticInfo1();
	void EnableDisableStaticInfo1(BOOL bState);
	CStatic m_stcFilename;
	CStatic m_stcSampleRate;
	CStatic m_stcBitrate;
	CStatic m_stcChannels;
	CStatic m_stcCodec;

	CI3Pack *m_i3Pack;

	CFile *m_pOGGFile;

	CButton m_btnBrowse2;
	CEdit m_editPath2;
	afx_msg void OnBnClickedBtnbrowse2();
	BOOL IsFileExists(CString csFile);
	CStatic m_stcFilename2;
	CStatic m_stcSampleRate2;
	CStatic m_stcBitrate2;
	CStatic m_stcChannels2;
	CStatic m_stcCodec2;
	afx_msg void OnBnClickedBtnChange();
	void EnableDisableStaticInfo2(BOOL bState);
	void EmptyStaticInfo2();
	vector<CNodeFileInfo>::iterator *m_mNodeIter;

	CFileException fe;
	CButton m_btnChange;

	CString m_csOpenedPath, m_csOggPath;

	CStatic m_stcAuthor;

	CMyHyperLink m_authHyp;
};
