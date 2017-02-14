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

// DDSPatcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DDSPatcher.h"
#include "DDSPatcherDlg.h"
#include <Shlwapi.h>
#include "../i3PackTool/Crypt.h"
//#include <pespin.h>


#define SAFEBUFFER(v, sz)	boost::shared_ptr<BYTE> v((BYTE *)malloc(sz), free)

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


// CDDSPatcherDlg dialog




CDDSPatcherDlg::CDDSPatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDDSPatcherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CDDSPatcherDlg::~CDDSPatcherDlg() 
{
	if(m_pDDSBuffer)
		m_pDDSBuffer->reset();

	if(m_pI3IBuffer) {
		m_pI3IBuffer->reset();
	}
}

void CDDSPatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_AUTHOR, m_linkAuthor);
}

BEGIN_MESSAGE_MAP(CDDSPatcherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDDSPatcherDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDDSPatcherDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDDSPatcherDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDDSPatcherDlg message handlers

BOOL CDDSPatcherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);

	m_pDDSBuffer = 0;
	m_pI3IBuffer = 0;

//	CRYPT_START;
	CString csCrypt;
	NotXorBit((PBYTE)"\xE6\xE5\xF2\xED\xE6\xE1\xE6\xF5\xC7\xEE\xB4\xE4\xE8\xF5\xE2\x87" /*abujafar@i3core*/, 16, csCrypt, 0x78);
	m_linkAuthor.SetLinkText(csCrypt);

	NotXorBit((PBYTE)"\xEF\xF3\xF3\xF7\xBD\xA8\xA8\xE0\xE8\xE8\xA9\xE0\xEB\xA8\xE8\xFE\xED\xCB\xE1\x87" /*http://goo.gl/oyjLf*/, 20, csCrypt, 0x78);
	m_linkAuthor.SetLinkUrl(csCrypt);

	NotXorBit((PBYTE)"\xC3\xC3\xD4\xA7\xF3\xE8\xA7\xCE\xB4\xCE\xA7\xD7\xE6\xF3\xE4\xEF\xE2\xF5\x87" /*DDS to I3I Patcher*/, 19, csCrypt, 0x78);
	SetWindowText(csCrypt);

//	CRYPT_END;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDDSPatcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDDSPatcherDlg::OnPaint()
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
HCURSOR CDDSPatcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDDSPatcherDlg::OnBnClickedButton1() // file 1
{
	try {
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->SetWindowText(0);
		GetDlgItem(IDC_EDIT2)->SetWindowText(0);

		CFileDialog fileDlg(TRUE, "i3i", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, "Converted DDS Files (*.dds)|converted_*.dds||");
		if(fileDlg.DoModal() != IDOK)
			return;

		if(m_pDDSBuffer) {
			m_pDDSBuffer->reset();
			m_pDDSBuffer = 0;
		}

		CString csOpenedDDSFile = fileDlg.GetPathName();
		if(IsFileExists(csOpenedDDSFile) == FALSE)
			THROW("File does not exists.");

		CFile fileDDS;
		if(fileDDS.Open(csOpenedDDSFile, CFile::modeRead) == FALSE)
			THROW("Could not open DDS file.");

		DDS_HEADER *pDDS = new DDS_HEADER;
		ZeroMemory(pDDS, sizeof(DDS_HEADER));

		if(fileDDS.Read(pDDS, sizeof(DDS_HEADER)) != sizeof(DDS_HEADER))
			THROW("Could not read DDS file.");

		if(pDDS->dwHeader != 0x20534444)
			THROW("Your DDS file is invalid.");

		CFileStatus fs;
		if(fileDDS.GetStatus(fs) == FALSE)
			THROW("Could not get file status.");

		ULONG ulDDSBodySz = ULONG(fs.m_size)-sizeof(DDS_HEADER);

		// create DDS buffer
		m_pDDSBuffer = new boost::shared_ptr<BYTE>((BYTE*)malloc(ulDDSBodySz), free);

		ZeroMemory(m_pDDSBuffer->get(), ulDDSBodySz);

		if(fileDDS.Seek(sizeof(DDS_HEADER), CFile::begin) != sizeof(DDS_HEADER))
			THROW("Could not seek into DDS body file.");

		if(fileDDS.Read(m_pDDSBuffer->get(), ulDDSBodySz) != ulDDSBodySz)
			THROW("Could not read DDS body file.");

		
		GetDlgItem(IDC_EDIT1)->SetWindowText(csOpenedDDSFile);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);

		m_csOpenedDDSPath = csOpenedDDSFile;
		m_ulDDSBufferSize = ulDDSBodySz;

		delete pDDS;
		fileDDS.Close();

	}
	catch(char *e) {
		if(m_pDDSBuffer) {
			m_pDDSBuffer->reset();
			m_pDDSBuffer = 0;
		}

		m_ulDDSBufferSize = 0;

		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->SetWindowText(0);

		AfxMessageBox(e);
		return;
	}
}

void CDDSPatcherDlg::OnBnClickedButton2() // file 2
{
	I3I_HEADER *pI3I = 0;

	try
	{
		if(!m_pDDSBuffer)
			THROW("Please open DDS file first.");

		CString csI3IFile = PathFindFileName(m_csOpenedDDSPath);

		int nFindConv = csI3IFile.Find("converted_");
		if(nFindConv != -1) {
			csI3IFile.Replace("converted_", "");
		}
		
		char *pszTemp = new char[csI3IFile.GetLength()+1];
		ZeroMemory(pszTemp, csI3IFile.GetLength()+1);
		strcpy(pszTemp, csI3IFile.GetBuffer(0));
		PathRemoveExtension(pszTemp);
		
		csI3IFile = CString(pszTemp) + ".i3i";
		
		delete pszTemp;

		CString csFilter;
		csFilter.Format("Original I3I Files (*.i3i)|%s||", csI3IFile);

		CFileDialog fileDlg(TRUE, "i3i", csI3IFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, csFilter);
		if(fileDlg.DoModal() != IDOK)
			return;

		if(m_pI3IBuffer) {
			m_pI3IBuffer->reset();
			m_pI3IBuffer = 0;
		}

		CString csOpenedI3IFile = fileDlg.GetPathName();

		if(IsFileExists(csOpenedI3IFile) == FALSE)
			THROW("I3I file does not exists.");

		CFile fileI3I;
		if(fileI3I.Open(csOpenedI3IFile, CFile::modeRead) == FALSE)
			THROW("Could not open your I3I file.");

		pI3I = new I3I_HEADER;
		ZeroMemory(pI3I, sizeof(I3I_HEADER));

		fileI3I.SeekToBegin();

		if(fileI3I.Read(pI3I, sizeof(I3I_HEADER)) != sizeof(I3I_HEADER))
			THROW("Could not read #1 I3I header.");

		if(pI3I->ulMagic != 0x42493349)
			THROW("Your I3I file is invalid.");

		ULONG ulI3IHdrSz = sizeof(I3I_HEADER)+pI3I->usMarkerLen;
		m_pI3IBuffer = new boost::shared_ptr<BYTE>((BYTE *)malloc(ulI3IHdrSz), free);
		ZeroMemory(m_pI3IBuffer->get(), ulI3IHdrSz);

		fileI3I.SeekToBegin();

		if(fileI3I.Read(m_pI3IBuffer->get(), ulI3IHdrSz) != ulI3IHdrSz)
			THROW("Could not read #2 I3I header.");

		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT2)->SetWindowText(csOpenedI3IFile);

		m_csOpenedI3IPath = csOpenedI3IFile;
		m_ulI3IBufferSize = ulI3IHdrSz;

		delete pI3I;
		fileI3I.Close();
	}
	catch (char* e)
	{
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT2)->SetWindowText(0);

		if(m_pI3IBuffer) {
			m_pI3IBuffer->reset();
			m_pI3IBuffer = 0;
		}
		
		m_ulI3IBufferSize = 0;


		if(pI3I)
			delete pI3I;

		AfxMessageBox(e);
		return;
	}
}

void CDDSPatcherDlg::OnBnClickedButton3() // patch
{
	try
	{
		if(!m_pDDSBuffer || !m_pI3IBuffer)
			THROW("DDS or I3I file are not initialized yet.");

		CFile fileWrite;
		if(fileWrite.Open(m_csOpenedI3IPath, CFile::modeCreate | CFile::modeWrite) == FALSE)
			THROW("Could not open/update I3I file.");

		//fileWrite.SeekToBegin();
		fileWrite.Write(m_pI3IBuffer->get(), m_ulI3IBufferSize);

		//fileWrite.SeekToEnd();
		fileWrite.Write(m_pDDSBuffer->get(), m_ulDDSBufferSize);

		fileWrite.Close();

		AfxMessageBox("Your I3I file has been update. Enjoy!", MB_ICONINFORMATION);

	}
	catch (char* e)
	{
		AfxMessageBox(e);
		return;
	}
}


BOOL CDDSPatcherDlg::IsFileExists( CString csFilename )
{
	CFileStatus fs;
	return CFile::GetStatus(csFilename, fs);
}