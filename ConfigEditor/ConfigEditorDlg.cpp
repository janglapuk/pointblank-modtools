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

// ConfigEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigEditor.h"
#include "ConfigEditorDlg.h"
/*#include "CryptedFunc.h"*/
#include <boost/shared_ptr.hpp>
#include "../i3PackTool/Crypt.h"

#define SAFEBUFFER(v, sz)	boost::shared_ptr<BYTE> v((BYTE *)malloc(sz), free)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


typedef void (__cdecl *pBitRotate)(void *, int, int);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CConfigEditorDlg dialog




CConfigEditorDlg::CConfigEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfigEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnBrowse);
	DDX_Control(pDX, IDC_BUTTON2, m_btnSave);
	DDX_Control(pDX, IDC_EDIT1, m_editPath);
	DDX_Control(pDX, IDC_EDIT2, m_editContent);
	DDX_Control(pDX, IDC_STC_AUTHOR, m_linkAuth);
}

BEGIN_MESSAGE_MAP(CConfigEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CConfigEditorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CConfigEditorDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CConfigEditorDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CConfigEditorDlg message handlers

BOOL CConfigEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	lpCF = 0;

	CString csBuf;
	NotXorBit((PBYTE)"\xF1\xED\xED\xE9\xA3\xB6\xB6\xFE\xF6\xF6\xB7\xFE\xF5\xB6\xDE\xAB\xD0\xEF\xAA\x99" /*http://goo.gl/G2Iv3*/, 20, csBuf, 0x66);
	m_linkAuth.SetLinkUrl(csBuf);

	NotXorBit((PBYTE)"\xF8\xFB\xEC\xF3\xF8\xFF\xF8\xEB\xB9\x30\xB9\xAB\xA9\xA8\xAA\x99" /*abujafar © 2013*/, 16, csBuf, 0x66);
	m_linkAuth.SetLinkText(csBuf);

	NotXorBit((PBYTE)"\xC9\xDB\xB9\xDA\xF6\xF7\xFF\xF0\xFE\xB9\xDC\xFD\xF0\xED\xF6\xEB\x99" /*PB Config Editor*/, 17, csBuf, 0x66);
	SetWindowText(csBuf);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CConfigEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CConfigEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CConfigEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CConfigEditorDlg::OnBnClickedButton1()
{
	CFile *readFile = 0;
	try
	{
		CFileDialog fileDlg(TRUE, "sif", "lwsi_En.sif", OFN_FILEMUSTEXIST, "Config File (*.sif)|*.sif|All Files (*.*)|*.*||");
		if(fileDlg.DoModal() != IDOK)
			return;
		
		m_csOpenedPath = fileDlg.GetPathName();
		m_editPath.SetWindowText(m_csOpenedPath);

		readFile = new CFile;
		if(readFile->Open(m_csOpenedPath, CFile::modeRead) == FALSE)
			THROW("Could not open config file.");

		CFileStatus fs;
		if(readFile->GetStatus(fs) == FALSE)
			THROW("Could not get file status/size.");

		ULONG ulFS = (ULONG)fs.m_size;

		SAFEBUFFER(pBuffer, ulFS+1);
		ZeroMemory(pBuffer.get(), ulFS+1);

		if(readFile->Read(pBuffer.get(), ulFS) != ulFS) 
			THROW("Could not read config file.");

		BitRotateDecrypt(pBuffer.get(), ulFS, 7);
		
		CString csConfig = CString((char *)pBuffer.get());

		m_editContent.SetWindowText(csConfig);
				
		readFile->Close();
		delete readFile;
		pBuffer.reset();
	}
	catch (char *e)
	{
		if(readFile) {
			if(readFile->m_hFile != INVALID_HANDLE_VALUE)
				readFile->Close();
			delete readFile;
		}
		AfxMessageBox(e);
	}
}

void CConfigEditorDlg::OnBnClickedButton2()
{
	CFile *writeFile = 0;
	try
	{
		int nConfigLen = m_editContent.GetWindowTextLength();

		if(nConfigLen <= 0)
			THROW("Your config content was null. Save ignored.");

		SAFEBUFFER(pBuffer, nConfigLen+1);
		ZeroMemory(pBuffer.get(), nConfigLen+1);

		int nRetCopy = m_editContent.GetWindowText((LPTSTR)pBuffer.get(), nConfigLen+1);
		if(nRetCopy != nConfigLen)
			THROW("Your config could not be copied. Save ignored.");

		BitRotateEncrypt(pBuffer.get(), nRetCopy, 7);

		writeFile = new CFile;
		if(writeFile->Open(m_csOpenedPath, CFile::modeCreate | CFile::modeWrite) == FALSE)
			THROW("Could not create/write your config. Please make sure your config file not opened by another program.");
		
		writeFile->Write(pBuffer.get(), nRetCopy);

		writeFile->Close();
		delete writeFile;

		AfxMessageBox("Your config has been saved.", MB_ICONINFORMATION);
		
	}
	catch (char *e)
	{
		if(writeFile) {
			if(writeFile->m_hFile != INVALID_HANDLE_VALUE)
				writeFile->Close();
			delete writeFile;
		}
		AfxMessageBox(e);
	}
}

// void *CConfigEditorDlg::BitRotateEncrypt() {
// 	try
// 	{
// 		lpCF = malloc(0x68);
// 		if(!lpCF)
// 			THROW(0);
// 	
// 		ZeroMemory(lpCF, 0x68);
// 	
// 		if(memcpy_s(lpCF, 0x68, &ucCryptedBRE, 0x68) != 0)
// 			THROW(0);
// 	
// 		if(!CryptedFunc::NotXorBit((BYTE *)lpCF, 0x68))
// 			THROW(0);
// 	
// 	}
// 	catch (char *e)
// 	{
// 		//(void)e;
// 		BitRotateCleanup();
// 		return 0;
// 	}
// 	return lpCF;
// }
// 
// void *CConfigEditorDlg::BitRotateDecrypt() {
// 	try
// 	{
// 		lpCF = malloc(0x60);
// 		if(!lpCF)
// 			THROW(0);
// 
// 		ZeroMemory(lpCF, 0x60);
// 
// 		if(memcpy_s(lpCF, 0x60, &ucCryptedBRD, 0x60) != 0)
// 			THROW(0);
// 
// 		if(!CryptedFunc::NotXorBit((BYTE *)lpCF, 0x60))
// 			THROW(0);
// 
// 	}
// 	catch (char *e)
// 	{
// 		//(void)e;
// 		BitRotateCleanup();
// 		return 0;
// 	}
// 	return lpCF;
// }
// 
// void CConfigEditorDlg::BitRotateCleanup()
// {
// 	if(lpCF) {
// 		free(lpCF);
// 		lpCF = 0;
// 	}
// }

void CConfigEditorDlg::OnBnClickedButton3()
{
	//OutputDebugString("[INFO] Select all\r\n");
	m_editContent.SetFocus();
	m_editContent.SetSel(0, -1);

}
