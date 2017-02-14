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


// i3PackToolDlg.h : header file
//

#pragma warning(disable: 4996)

#pragma once
// #include "afxcmn.h"
// #include "afxwin.h"

#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

#include "Structures.h"
#include "../BGMChanger/CI3Pack.h"

using namespace std;

#define SAFEBUFFER(v, sz)	boost::shared_ptr<BYTE> v((BYTE *)malloc(sz), free)

// Ci3PackToolDlg dialog
class Ci3PackToolDlg : public CDialog
{
// Construction
public:
	Ci3PackToolDlg(CWnd* pParent = NULL);	// standard constructor

	~Ci3PackToolDlg();

// Dialog Data
	enum { IDD = IDD_I3PACKTOOL_DIALOG };

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
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTvnSelchangedTreenode(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
	void InitListControl();
	void InitTreeNodeControl();

	vector<CString> split(CString& str, char* pszSeps);
	
	vector<CSingleNode> m_vecNodes;

	CFile m_i3File;

	boost::shared_ptr<BYTE> m_pNodeInfoBuffer;

	CSingleNode *m_pSelectedNode;

	CI3Pack m_I3Pack;
	
public:
	CTreeCtrl m_treeNode;
	CListCtrl m_listContent;
	afx_msg void OnListctlProperties();
	CEdit m_editFilePath;
	afx_msg void OnBnClickedBtnbrowse();
	void DoOpeni3PackFile(CString csFile=0);
	void ExpandAllTree(CTreeCtrl *pCtl);
	CNodeInfo * GetNodeInfoById(void *lpBase, int nSize, int nIndex);
	void SetSBFileCount(int nCount);
	CString m_strOpenedPath;

	CStatusBar m_StatusBar;
	afx_msg void OnListctlDump();
	afx_msg void OnListctlDumpallfiles();
	BOOL SetClipboardText(CString &szData);
	afx_msg void OnListctlCopyDetails();
	afx_msg void OnListctlReplaceFileWith();
	BOOL IsFileExists(CString csFile);

	char m_szFormat[MAX_PATH * 2];
};
