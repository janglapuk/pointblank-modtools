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

// DeBitRotateDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "NumSpinCtrl.h"
#include <boost/shared_ptr.hpp>

#define SAFEBUFFER(v, sz)	boost::shared_ptr<BYTE> v((BYTE *)malloc(sz), free)

// CDeBitRotateDlg dialog
class CDeBitRotateDlg : public CDialog
{
// Construction
public:
	CDeBitRotateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEBITROTATE_DIALOG };
	CNumSpinCtrl m_numSpinKey;

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
	afx_msg void OnBnClickedRdoencrypt();
	afx_msg void OnBnClickedRdodecrypt();
	afx_msg void OnDeltaposSpinkey(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_editPathTarget;
	CEdit m_editPathDest;
	CButton m_btnBrowse;
	afx_msg void OnBnClickedBtnbrowse();
	afx_msg void OnBnClickedBtnencdec();
	void UpdateSpinValue();
	int m_rdoEncDec;
	CString m_csOpenedPath, m_csWriteDestFile;
	CButton m_btnEncDec;
};