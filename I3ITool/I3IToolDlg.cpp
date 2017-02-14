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

// I3IToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "I3ITool.h"
#include "I3IToolDlg.h"
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
/*#include <memory>*/
#include "../i3PackTool/Crypt.h"
//#include <pespin.h>


#define SAFEBUFFER(v, sz)	boost::shared_ptr<BYTE> v((BYTE *)malloc(sz), free)

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                  \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

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


// CI3IToolDlg dialog




CI3IToolDlg::CI3IToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CI3IToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CI3IToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_DROPHERE, m_stcDropHere);
	DDX_Control(pDX, IDC_COMBO1, m_cboFiles);
	DDX_Control(pDX, IDC_STC_AUTHOR, m_linkAuthor);
}

BEGIN_MESSAGE_MAP(CI3IToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DROPFILES, OnDropFiles)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO1, &CI3IToolDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BTN_CONVSINGLE, &CI3IToolDlg::OnBnClickedBtnConvsingle)
END_MESSAGE_MAP()


// CI3IToolDlg message handlers

BOOL CI3IToolDlg::OnInitDialog()
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
	DragAcceptFiles(TRUE);

	m_Font.CreatePointFont(130, _T("Arial Black"));
	
	m_stcDropHere.SetFont(&m_Font);
	m_stcDropHere.SetWindowText("DROP FILES HERE");

	memset(&m_szFormat, 0, MAX_PATH*2);

	// link
//	CRYPT_START;

	CString csCrypt;

	NotXorBit((PBYTE)"\xE6\xE5\xF2\xED\xE6\xE1\xE6\xF5\xC7\xEE\xB4\xE4\xE8\xF5\xE2\x87" /*abujafar@i3core*/, 16, csCrypt, 0x78);
	m_linkAuthor.SetLinkText(csCrypt);

	NotXorBit((PBYTE)"\xEF\xF3\xF3\xF7\xBD\xA8\xA8\xE0\xE8\xE8\xA9\xE0\xEB\xA8\xDF\xD0\xE0\xE0\xD5\x87" /*http://goo.gl/XWggR*/, 20, csCrypt, 0x78);
	m_linkAuthor.SetLinkUrl(csCrypt);
	
	NotXorBit((PBYTE)"\xCE\xB4\xCE\xD3\xE8\xE8\xEB\x87" /*I3ITool*/, 8, csCrypt, 0x78);
	SetWindowText(csCrypt);

//	CRYPT_END;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CI3IToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CI3IToolDlg::OnPaint()
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
HCURSOR CI3IToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CI3IToolDlg::OnDropFiles( WPARAM wParam, LPARAM lParam )
{
	char szBuffer[MAX_PATH] = {0};

	UINT uiCount = DragQueryFile((HDROP)wParam, -1, NULL, NULL);

	m_vFileList.clear();
	m_cboFiles.ResetContent();

	if(uiCount) {
		for(UINT i=0; i < uiCount; i++) {
			DragQueryFile((HDROP)wParam, i, szBuffer, MAX_PATH);
			if(szBuffer != NULL)
				InsertToVector(szBuffer);

			if(!m_vFileList.empty())
				m_cboFiles.EnableWindow(TRUE);
			else
				m_cboFiles.EnableWindow(FALSE);
		}
	}

	DragFinish((HDROP)wParam);

	FillComboList();

	UpdateInfo();

	return 0;
}

void CI3IToolDlg::InsertToVector(CString csFile) {
	try {
		if(IsFileExists(csFile) == FALSE) {
			sprintf_s(m_szFormat, MAX_PATH*2, "File is not exists.\r\n\r\n%s", csFile);
			THROW(m_szFormat);
		}

		CFile openFile;
		if(openFile.Open(csFile, CFile::modeRead) == FALSE) {
			CString s;
			sprintf_s(m_szFormat, MAX_PATH*2, "Could open file.\r\n\r\n%s", csFile);
			THROW(m_szFormat);
		}

		CString csFileExt = PathFindExtension(csFile);
		csFileExt.MakeLower();

		ULONG ulWidth = 0;
		ULONG ulHeight = 0;
		ULONG ulFormat = 0;
		ULONG ulMipMapCount = 0;
		ULONG ulMarkerLen = 0;
		CString csMarker("");

		if(csFileExt == ".dds") {
			SAFEBUFFER(pBuffer, sizeof(DDS_HEADER));
			ZeroMemory(pBuffer.get(), sizeof(DDS_HEADER));

			openFile.Seek(0, CFile::begin);
			if(openFile.Read(pBuffer.get(), sizeof(DDS_HEADER)) != sizeof(DDS_HEADER)) {
				sprintf_s(m_szFormat, MAX_PATH*2, "Could not read DDS file.\r\n\r\n%s", csFile);
				THROW(m_szFormat);
			}

			DDS_HEADER *pDDS = (DDS_HEADER *)pBuffer.get();
			
			ulWidth = pDDS->dwWidth;
			ulHeight = pDDS->dwHeight;
			ulFormat = pDDS->ddspf.dwFourCC;
			ulMipMapCount = pDDS->dwMipMapCount;

		} else if(csFileExt == ".i3i") {
			SAFEBUFFER(pBuffer, sizeof(I3I_HEADER));
			ZeroMemory(pBuffer.get(), sizeof(I3I_HEADER));
			openFile.Seek(0, CFile::begin);
			if(openFile.Read(pBuffer.get(), sizeof(I3I_HEADER)) != sizeof(I3I_HEADER)) {
				sprintf_s(m_szFormat, MAX_PATH*2, "Could not read I3I file.\r\n\r\n%s", csFile);
				THROW(m_szFormat);
			}

			I3I_HEADER *pI3I = (I3I_HEADER *)pBuffer.get();

			ulWidth = (ULONG)pI3I->usWidth;
			ulHeight = (ULONG)pI3I->usHeight;
			ulFormat = pI3I->ulFormat;
			ulMipMapCount = pI3I->usMipsLevel;

			ulMarkerLen = pI3I->usMarkerLen;
			if(ulMarkerLen) {
				openFile.Seek(sizeof(I3I_HEADER), CFile::begin);
				char *pszMarker = new char[ulMarkerLen+1];
				openFile.Read(pszMarker, ulMarkerLen);
				
				pszMarker[ulMarkerLen] = '\0';
				csMarker = CString(pszMarker);
				
				delete pszMarker;
			}

		} else {
			sprintf_s(m_szFormat, MAX_PATH*2, "Invalid file format.\r\n\r\n%s", csFile);
			THROW(m_szFormat);
		}
		
		// vec
		m_vFileList.push_back(I3IFileList(csFile, ulHeight, ulWidth, ulFormat, ulMipMapCount, ulMarkerLen, csMarker));

		openFile.Close();

	} catch(char *e) {
		AfxMessageBox(e);
	}
}

void CI3IToolDlg::FillComboList() {
	if(!m_vFileList.empty()) {
		for(vector<I3IFileList>::iterator it=m_vFileList.begin(); it != m_vFileList.end(); ++it) {
			m_cboFiles.AddString(it->csFilename);
		}
		m_cboFiles.SetCurSel(0);
	}
}

BOOL CI3IToolDlg::IsFileExists(CString csFile) {
	CFileStatus fs;
	return CFile::GetStatus(csFile, fs);
}
void CI3IToolDlg::OnCbnSelchangeCombo1()
{
	UpdateInfo();
}

void CI3IToolDlg::UpdateInfo() {
	int nIndex = m_cboFiles.GetCurSel();

	if(nIndex >= 0) {
		m_cboFiles.GetLBText(nIndex, m_szFormat);

		CString csSelFn = CString(m_szFormat);

		vector<I3IFileList>::iterator it;

		it = find_if(m_vFileList.begin(), 
					 m_vFileList.end(), 
					 boost::BOOST_BIND(&I3IFileList::csFilename, _1) == csSelFn
					);

		if(it != m_vFileList.end()) {
			CString s;
			s.Format("%d px", it->ulWidth);
			GetDlgItem(IDC_STC_WIDTH)->SetWindowText(s);

			s.Format("%d px", it->ulHeight);
			GetDlgItem(IDC_STC_HEIGHT)->SetWindowText(s);

			s.Format("%d", it->ulMipMapCount);
			GetDlgItem(IDC_STC_MIPMAPCOUNT)->SetWindowText(s);

			GetDlgItem(IDC_STC_FORMAT)->SetWindowText(EnumerateFormat(it->ulFormat));

			char *pszFn = new char[it->csFilename.GetLength()+1];
			strcpy(pszFn, it->csFilename.GetBuffer(0));
			PathRemoveExtension(pszFn);
			s = "converted_";
			s += CString(pszFn);
			s += it->nDirection == 1 ? ".i3i" : ".dds";
			GetDlgItem(IDC_STC_OUTPUTAS)->SetWindowText(s);

			it->csSavedFilename = s;
			
			delete pszFn;
		}
	}
}

CString CI3IToolDlg::EnumerateFormat(ULONG ulFmt) {
	CString s;
	switch(ulFmt)
	{
		case 0x31545844:
		case 0xA0000681:
		case 0x80000680:
			s = "DXT1"; break;
		
// 		case 0xA0000601:
// 		case 0x32545844:
// 			s = "DXT2"; break;
// 		
		case 0xA0000602:
		case 0x33545844:
			s = "DXT3"; break;
// 		
// 		case 0xA0000603:
// 		case 0x34545844:
// 			s = "DXT4"; break;
		
		case 0xA0000604:
		case 0x35545844:
			s = "DXT5"; break;

// 		case 0x20000204:
// 			s = "ARGB 1555"; break;
// 
// 		case 0x10000102:
// 			s = "8"; break;
// 
// 		case 0x2000020A:
// 			s = "ABGR 4444"; break;

		default:
			s = "UNKNOWN"; break;
	}
	return s;
}

DDS_HEADER * CI3IToolDlg::GenerateDDSHeader( ULONG pf, DWORD dwHeight, DWORD dwWidth, DWORD dwMipMapCount/*=0*/ )
{
	DDS_HEADER *header = new DDS_HEADER;
	
	ZeroMemory(header, sizeof(DDS_HEADER));

	header->dwHeader			= 0x20534444;
	header->dwSize				= 0x7C;
	header->dwHeight			= dwHeight;
	header->dwWidth				= dwWidth;

	header->ddspf.dwSize		= sizeof(DDS_PIXELFORMAT);
	header->ddspf.dwFlags		= 0x4;

	header->dwCaps				= 0x1000;

	if(dwMipMapCount) {
		header->dwMipMapCount	= dwMipMapCount;
	}


	switch(pf) {
		case DDS_PF_DXT1:
			header->dwFlags				= 0x81007;
			header->dwPitchOrLinearSize	= 0x8000;
			header->ddspf.dwFourCC		= MAKEFOURCC('D', 'X', 'T', '1');	// DXT1
			break;

// 		case DDS_PF_DXT1_ALPHA:
// 			header.dwFlags				= 0x81007;
// 			header.dwPitchOrLinearSize	= 0x8000;
// 			header.ddspf.dwFourCC		= MAKEFOURCC('D', 'X', 'T', '1');	// DXT1
// 			break;

		case DDS_PF_DXT3:
			header->dwFlags				= 0x81007;
			header->dwPitchOrLinearSize	= 0x8000;
			header->ddspf.dwFourCC		= MAKEFOURCC('D', 'X', 'T', '3');	// DXT1
			break;

		case DDS_PF_DXT5_INTERP:
			header->dwFlags				= 0x81007;
			header->dwPitchOrLinearSize	= 0x10000;
			header->ddspf.dwFourCC		= MAKEFOURCC('D', 'X', 'T', '5');	// DXT5
			break;

		default:
			delete header;
			return NULL;
	}

	return header;
}
void CI3IToolDlg::OnBnClickedBtnConvsingle()
{
	DDS_HEADER *pDDS = 0;

	try 
	{
		int nIndex = m_cboFiles.GetCurSel();

		if(nIndex >= 0) {
			m_cboFiles.GetLBText(nIndex, m_szFormat);

			CString csSelFn = CString(m_szFormat);

			vector<I3IFileList>::iterator it;

			it = find_if(m_vFileList.begin(), 
				m_vFileList.end(), 
				boost::BOOST_BIND(&I3IFileList::csFilename, _1) == csSelFn
				);

			if(it == m_vFileList.end()) 
				THROW("File not found.");

			if(it->nDirection == it->ToDDS) {
				ULONG ulFmt;

				CString csFormat = EnumerateFormat(it->ulFormat);
				if(csFormat == "DXT1") {
					ulFmt = DDS_PF_DXT1;
				} else if(csFormat == "DXT3") {
					ulFmt = DDS_PF_DXT3;
				} else if(csFormat == "DXT5") {
					ulFmt = DDS_PF_DXT5_INTERP;
				} else {
					ulFmt = -1;
				}
				
				pDDS = GenerateDDSHeader(ulFmt, it->ulHeight, it->ulWidth, it->ulMipMapCount);

				if(!pDDS)
					THROW("Could not generate file header.");

				CFile fileDDS;
				if(fileDDS.Open(it->csFullPath, CFile::modeRead) == FALSE)
					THROW("Could not open selected file.");

				CFileStatus fs;
				if(fileDDS.GetStatus(fs) == FALSE)
					THROW("Could not get file status.");

				ULONG ulFsize = (ULONG)fs.m_size;

				ULONG ulBodyOffs = sizeof(I3I_HEADER)+it->ulMarketLen;
				ULONG ulBodySize = ulFsize-ulBodyOffs;

				fileDDS.Seek(ulBodyOffs, CFile::begin);

				SAFEBUFFER(pBuffer, ulBodySize);
				ZeroMemory(pBuffer.get(), ulBodySize);

				if(fileDDS.Read(pBuffer.get(), ulBodySize) != ulBodySize)
					THROW("Could not read DDS body buffer.");

				ULONG ulPathsz = it->csFullPath.GetLength()+1;
				
				char szSaved[MAX_PATH] = {0};

				strcpy(szSaved, it->csFullPath.GetBuffer(0));
				szSaved[ulPathsz] = '\0';

				PathRemoveFileSpec(szSaved);

				CString csSavedFile(szSaved);

				strcpy(szSaved, it->csFilename.GetBuffer(0));
				szSaved[it->csFilename.GetLength()] = '\0';

				PathRemoveExtension(szSaved);
				
				csSavedFile += "\\converted_";
				csSavedFile += CString(szSaved);
				csSavedFile += ".dds";


				CFile fileWrite;
				if(fileWrite.Open(csSavedFile, CFile::modeCreate | CFile::modeWrite) == FALSE)
					THROW("Could not create DDS file.");

				fileWrite.Write(pDDS, sizeof(DDS_HEADER));
				fileWrite.SeekToEnd();
				fileWrite.Write(pBuffer.get(), ulBodySize);

				pBuffer.reset();
				fileDDS.Close();

				CString s;
				s.Format("Your I3I has been converted into a new file at:\n\n%s", csSavedFile);
				AfxMessageBox(s, MB_ICONINFORMATION);

			} else {
				AfxMessageBox("Sorry! Could not convert the DDS into I3I for a while.");
			}
		}
	} 
	catch(char *e) 
	{
		if(pDDS)
			delete pDDS;
		AfxMessageBox(e);
		return;
	}
	if(pDDS)
		delete pDDS;
}
