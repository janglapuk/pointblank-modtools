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


// DeBitRotateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DeBitRotate.h"
#include "DeBitRotateDlg.h"

#include "../i3PackTool/Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CDeBitRotateDlg dialog




CDeBitRotateDlg::CDeBitRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeBitRotateDlg::IDD, pParent)
	, m_rdoEncDec(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeBitRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDTTARGET, m_editPathTarget);
	DDX_Control(pDX, IDC_EDTDEST, m_editPathDest);
	DDX_Control(pDX, IDC_BTNBROWSE, m_btnBrowse);
	DDX_Control(pDX, IDC_SPINKEY, m_numSpinKey);
	DDX_Radio(pDX, IDC_RDOENCRYPT, m_rdoEncDec);
	DDX_Control(pDX, IDC_BTNENCDEC, m_btnEncDec);
}

BEGIN_MESSAGE_MAP(CDeBitRotateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RDOENCRYPT, &CDeBitRotateDlg::OnBnClickedRdoencrypt)
	ON_BN_CLICKED(IDC_RDODECRYPT, &CDeBitRotateDlg::OnBnClickedRdodecrypt)
	ON_BN_CLICKED(IDC_BTNBROWSE, &CDeBitRotateDlg::OnBnClickedBtnbrowse)
	ON_BN_CLICKED(IDC_BTNENCDEC, &CDeBitRotateDlg::OnBnClickedBtnencdec)
END_MESSAGE_MAP()


// CDeBitRotateDlg message handlers

BOOL CDeBitRotateDlg::OnInitDialog()
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
	m_numSpinKey.SetDecimalPlaces (3);
	m_numSpinKey.SetTrimTrailingZeros (FALSE);
	m_numSpinKey.SetRangeAndDelta (0, 9, 1);
	m_numSpinKey.SetTrimTrailingZeros(TRUE);
	m_numSpinKey.SetPos(4);

	m_rdoEncDec = 0;

	m_btnEncDec.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDeBitRotateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDeBitRotateDlg::OnPaint()
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
HCURSOR CDeBitRotateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDeBitRotateDlg::OnBnClickedRdoencrypt()
{
	m_editPathTarget.SetWindowText(0);
	m_editPathDest.SetWindowText(0);
	m_btnEncDec.SetWindowText("&Encrypt");
	m_btnEncDec.EnableWindow(FALSE);
}

void CDeBitRotateDlg::OnBnClickedRdodecrypt()
{
	m_editPathTarget.SetWindowText(0);
	m_editPathDest.SetWindowText(0);
	m_btnEncDec.SetWindowText("&Decrypt");
	m_btnEncDec.EnableWindow(FALSE);
}

void CDeBitRotateDlg::OnBnClickedBtnbrowse()
{
	try 
	{
		UpdateData();
		UpdateSpinValue();
	
		CFileDialog fileDlg(TRUE, "", "", OFN_READONLY | OFN_FILEMUSTEXIST, m_rdoEncDec ? "Encrypted Files (*.bre)|*.bre|All Files (*.*)|*.*||" : "Decrypted Files (*.brd)|*.brd|All Files (*.*)|*.*||");
		if(fileDlg.DoModal() != IDOK)
			return;

		m_csOpenedPath = fileDlg.GetPathName();

		CString csDestpath = m_csOpenedPath;

		int nFind = 0;
		if((nFind = csDestpath.Find(".brd")) != -1 || (nFind = csDestpath.Find(".brd")) != -1) {
			csDestpath = csDestpath.Mid(0, nFind);
		}

		m_csWriteDestFile = m_rdoEncDec ? csDestpath + ".brd" : csDestpath + ".bre";
	
		m_editPathTarget.SetWindowText(m_csOpenedPath);
		m_editPathDest.SetWindowText(m_csWriteDestFile);
		m_btnEncDec.EnableWindow(TRUE);
		
	}
	catch (CString e)
	{
		AfxMessageBox(e);
	}
}

void CDeBitRotateDlg::OnBnClickedBtnencdec()
{
	try
	{
		UpdateData();
		UpdateSpinValue();

		CFile readFile;
		if(readFile.Open(m_csOpenedPath, CFile::modeRead) == FALSE)
			THROW("Could not open file.");

		CFileStatus fs;
		if(readFile.GetStatus(fs) == FALSE) {
			readFile.Close();
			THROW("Could not get file status.");
		}

		ULONG ulFileSize = (ULONG)fs.m_size;

		SAFEBUFFER(pBuffer, ulFileSize);

		if(readFile.Read(pBuffer.get(), ulFileSize) != ulFileSize) {
			readFile.Close();
			THROW("Could not read entire file.");
		}
		
		readFile.Close();

		if(m_rdoEncDec) 
			BitRotateDecrypt(pBuffer.get(), ulFileSize, (ULONG)m_numSpinKey.GetPos());
		else
			BitRotateEncrypt(pBuffer.get(), ulFileSize, (ULONG)m_numSpinKey.GetPos());

		CFile writeFile;
		if(writeFile.Open(m_csWriteDestFile, CFile::modeCreate | CFile::modeWrite) == FALSE)
			THROW("Could not create file.");

		writeFile.Write(pBuffer.get(), ulFileSize);
		writeFile.Close();
		pBuffer.reset();

		CFile writeLog;
		if(writeLog.Open(m_csWriteDestFile + ".log", CFile::modeCreate | CFile::modeWrite) == FALSE)
			AfxMessageBox("Log file could not to be created, but encrypt/decrypt process are successfully.");
		else {
			CString s;
			s.Format("Using key: %d", (ULONG)m_numSpinKey.GetPos());
			writeLog.Write(s.GetBuffer(0), s.GetLength());
			writeLog.Close();
		}

		AfxMessageBox(m_rdoEncDec ? "File has been decrypted!" : "File has been encrypted!", MB_ICONINFORMATION);
	}
	catch (CString e)
	{
		AfxMessageBox(e);
	}
}


void CDeBitRotateDlg::UpdateSpinValue() {
	m_numSpinKey.SetPos(m_numSpinKey.GetPos());
}

/*
__declspec(dllexport)
void __cdecl _BRE(void *pBuffer, int nCount, int nKey) {
	Crypt::BitRotateEncrypt(pBuffer, nCount, nKey);
}

__declspec(dllexport)
void __cdecl _BRD(void *pBuffer, int nCount, int nKey) {
	Crypt::BitRotateDecrypt(pBuffer, nCount, nKey);
}
*/