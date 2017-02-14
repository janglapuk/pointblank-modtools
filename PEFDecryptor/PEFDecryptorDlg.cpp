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


// PEFDecryptorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PEFDecryptor.h"
#include "PEFDecryptorDlg.h"

#include "../i3PackTool/Crypt.h"
#include <Shlwapi.h>

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


// CPEFDecryptorDlg dialog




CPEFDecryptorDlg::CPEFDecryptorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPEFDecryptorDlg::IDD, pParent)
	, m_rdoEncDec(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEFDecryptorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDTBROWSE, m_editBrowse);
	DDX_Control(pDX, IDC_BTNBROWSE, m_btnBrowse);
	DDX_Control(pDX, IDC_BTNENCDEC, m_btnEncDec);
	DDX_Control(pDX, IDC_EDTDEST, m_editDest);
	DDX_Radio(pDX, IDC_RDOENCRYPT, m_rdoEncDec);
}

BEGIN_MESSAGE_MAP(CPEFDecryptorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNBROWSE, &CPEFDecryptorDlg::OnBnClickedBtnbrowse)
	ON_BN_CLICKED(IDC_BTNENCDEC, &CPEFDecryptorDlg::OnBnClickedBtnEncDec)
	ON_BN_CLICKED(IDC_RDOENCRYPT, &CPEFDecryptorDlg::OnBnClickedRdoEncrypt)
	ON_BN_CLICKED(IDC_RDODECRYPT, &CPEFDecryptorDlg::OnBnClickedRdoDecrypt)
END_MESSAGE_MAP()


// CPEFDecryptorDlg message handlers

BOOL CPEFDecryptorDlg::OnInitDialog()
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
	m_rdoEncDec = 1;

	((CButton *)GetDlgItem(IDC_RDOENCRYPT))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_RDODECRYPT))->SetCheck(BST_CHECKED);

	m_btnEncDec.EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPEFDecryptorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPEFDecryptorDlg::OnPaint()
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
HCURSOR CPEFDecryptorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPEFDecryptorDlg::OnBnClickedBtnbrowse()
{
	try
	{
		UpdateData();

		CFileDialog fileDlg(TRUE, m_rdoEncDec ? "pef" : "dec", "", OFN_READONLY | OFN_FILEMUSTEXIST, m_rdoEncDec ? "PEF File (*.pef)|*.pef||" : "PEF.DEC File (*.pef.dec)|*.pef.dec||");
		if(fileDlg.DoModal() != IDOK)
			return;
		
		m_csPathFile = fileDlg.GetPathName();

		CString csFilePath = m_csPathFile;
		
		if(!m_rdoEncDec) {
			char *pszPath = new char[csFilePath.GetLength() + 2];
			strcpy_s(pszPath, csFilePath.GetLength() + 2, csFilePath.GetBuffer(0));
			PathRemoveExtension(pszPath);
			csFilePath = pszPath;
			delete pszPath;
		}

		m_csPathFileDest = !m_rdoEncDec ? csFilePath + "-" : csFilePath + ".dec";

		m_editBrowse.SetWindowText(m_csPathFile);
		m_editDest.SetWindowText(m_csPathFileDest);

		m_btnEncDec.EnableWindow(TRUE);

	}
	catch(CString e)
	{
		AfxMessageBox(e);
	}
}

void CPEFDecryptorDlg::OnBnClickedBtnEncDec()
{
	try
	{
		UpdateData();

		if(m_csPathFile.IsEmpty() || m_csPathFileDest.IsEmpty())
			THROW("Target/destination file are not set.");

		CFile pefFile;
		if(pefFile.Open(m_csPathFile, CFile::modeRead) == FALSE)
			THROW("Could not open file: " + m_csPathFile);

		CFileStatus fs;
		if(pefFile.GetStatus(fs) == FALSE) {
			pefFile.Close();
			THROW("Could not get file status.");
		}

		ULONG ulFileSize = (ULONG)fs.m_size;

		SAFEBUFFER(pBuffer, ulFileSize);
		ZeroMemory(pBuffer.get(), ulFileSize);

		if(pefFile.Read(pBuffer.get(), ulFileSize) != ulFileSize) {
			pefFile.Close();
			THROW("Could not read file (return size are not equals).");
		}
		pefFile.Close();

		if(m_rdoEncDec)
			BitRotateDecrypt(pBuffer.get(), ulFileSize, 3);
		else
			BitRotateEncrypt(pBuffer.get(), ulFileSize, 3);

		CFile decFile;
		if(decFile.Open(m_csPathFileDest, CFile::modeCreate | CFile::modeWrite) == FALSE)
			THROW("Could not create file: " + m_csPathFileDest);

		decFile.Write(pBuffer.get(), ulFileSize);
		decFile.Close();

		pBuffer.reset();

		AfxMessageBox("File has been decrypted successfully!", MB_ICONINFORMATION);


	}
	catch(CString e)
	{
		AfxMessageBox(e);
	}
}

void CPEFDecryptorDlg::OnBnClickedRdoEncrypt()
{
	UpdateData();
	m_btnEncDec.SetWindowText("&Encrypt");
	m_editBrowse.SetWindowText(0);
	m_editDest.SetWindowText(0);

	m_btnEncDec.EnableWindow(FALSE);
}

void CPEFDecryptorDlg::OnBnClickedRdoDecrypt()
{
	UpdateData();
	m_btnEncDec.SetWindowText("&Decrypt");
	m_editBrowse.SetWindowText(0);
	m_editDest.SetWindowText(0);

	m_btnEncDec.EnableWindow(FALSE);
}
