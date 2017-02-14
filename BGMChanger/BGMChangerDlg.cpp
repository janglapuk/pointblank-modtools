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


// BGMChangerDlg.cpp : implementation file
//

#include "stdafx.h"

#include <Shlwapi.h>

#include "BGMChanger.h"
#include "BGMChangerDlg.h"

#include <boost/bind.hpp>

#include "Ogg.h"
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


// CBGMChangerDlg dialog




CBGMChangerDlg::CBGMChangerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBGMChangerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBGMChangerDlg::~CBGMChangerDlg()
{
	if(m_i3Pack) {
		if(m_i3Pack->IsFileOpened())
			m_i3Pack->Close();
		delete m_i3Pack;
		m_i3Pack = 0;
	}

	if(m_pOGGFile) {
		if(m_pOGGFile->m_hFile != INVALID_HANDLE_VALUE)
			m_pOGGFile->Close();

		delete m_pOGGFile;
		m_pOGGFile = 0;
	}
}

#pragma region
void CBGMChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNBROWSE, m_btnBrowse);
	DDX_Control(pDX, IDC_EDTPATH, m_editPath);
	DDX_Control(pDX, IDC_STC_FILE, m_stcFilename);
	DDX_Control(pDX, IDC_STC_SAMPLE, m_stcSampleRate);
	DDX_Control(pDX, IDC_STC_BITRATE, m_stcBitrate);
	DDX_Control(pDX, IDC_STC_CHANNELS, m_stcChannels);
	DDX_Control(pDX, IDC_STC_CODEC, m_stcCodec);
	DDX_Control(pDX, IDC_BTNBROWSE2, m_btnBrowse2);
	DDX_Control(pDX, IDC_EDTPATH2, m_editPath2);
	DDX_Control(pDX, IDC_STC_FILE2, m_stcFilename2);
	DDX_Control(pDX, IDC_STC_SAMPLE2, m_stcSampleRate2);
	DDX_Control(pDX, IDC_STC_BITRATE2, m_stcBitrate2);
	DDX_Control(pDX, IDC_STC_CHANNELS2, m_stcChannels2);
	DDX_Control(pDX, IDC_STC_CODEC2, m_stcCodec2);
	DDX_Control(pDX, IDC_BTN_CHANGE, m_btnChange);
	DDX_Control(pDX, IDC_STC_AUTHOR, m_authHyp);
}

BEGIN_MESSAGE_MAP(CBGMChangerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNBROWSE, &CBGMChangerDlg::OnBnClickedBtnbrowse)
	ON_BN_CLICKED(IDC_BTNBROWSE2, &CBGMChangerDlg::OnBnClickedBtnbrowse2)
	ON_BN_CLICKED(IDC_BTN_CHANGE, &CBGMChangerDlg::OnBnClickedBtnChange)
END_MESSAGE_MAP()


// CBGMChangerDlg message handlers

BOOL CBGMChangerDlg::OnInitDialog()
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

// 	CFont    m_boldFont;
// 	HGDIOBJ hFont = ::GetStockObject( DEFAULT_GUI_FONT ); 
// 	CWnd * pwnd = GetDlgItem(IDC_STC_FILE);
// 	if(pwnd != NULL) 
// 	{ 
// 		CFont* pFont = pwnd->GetFont();
// 		if( pFont ) {
// 			LOGFONT lf;
// 			pFont->GetLogFont( &lf ); 
// 			//lf.lfWeight = FW_BOLD; 
// 			//lf.lfHeight = 5;
// 			m_boldFont.CreateFontIndirect( &lf ); 
// 			pwnd->SetFont( &m_boldFont ); 
// 		} 
// 	}

	m_i3Pack = new CI3Pack;

	m_pOGGFile = new CFile;

	EnableDisableStaticInfo1(FALSE);
	EnableDisableStaticInfo2(FALSE);
	m_btnChange.EnableWindow(FALSE);

	m_btnBrowse2.EnableWindow(FALSE);

	// author
// 	if(!m_stcAuthor.Create("abujafar", WS_EX_TRANSPARENT | WS_GROUP | SS_LEFT | WS_VISIBLE | WS_CHILD, CRect(10, (10+0*30), 70, (30+0*30)), this))
// 		AfxMessageBox("Cannot create static.");

	m_authHyp.SetLinkUrl("http://goo.gl/G2Iv3");
	m_authHyp.SetLinkText("abujafar © 2013");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBGMChangerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBGMChangerDlg::OnPaint()
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
HCURSOR CBGMChangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#pragma endregion

void CBGMChangerDlg::OnBnClickedBtnbrowse()
{
	try 
	{
		CFileDialog fileDlg(TRUE, "i3Pack", "Sound_BackGround.i3Pack", 4 | 2, "i3Pack Files (*.i3Pack)|*.i3Pack||");
		if(fileDlg.DoModal() != IDOK)
			return;

		CString csFile = fileDlg.GetPathName();
		m_editPath.SetWindowText(csFile);

		m_csOpenedPath = csFile;

		m_i3Pack->ClearBufferList();
		
		if(m_i3Pack->IsFileOpened())
			m_i3Pack->Close();

		if(m_i3Pack->Open(csFile) > 0)
			THROW("Could not open file."); 

		vector<CSingleNode> *pvNodes = m_i3Pack->GetNodes();

		vector<CSingleNode>::iterator it;
		it = std::find_if(pvNodes->begin(), pvNodes->end(), boost::BOOST_BIND(&CSingleNode::NodeName, _1) == "BackGround");

		if(it == pvNodes->end())
			THROW("Could not find required node.");

		vector<CNodeFileInfo>::iterator itf;
		itf = std::find_if(it->Files.begin(), it->Files.end(), boost::BOOST_BIND(&CNodeFileInfo::Filename, _1) == "Menu_BGM.ogg");
		if(itf == it->Files.end())
			THROW("Could not find required file on i3Pack");

		SAFEBUFFER(OggBuf, 255);
		ZeroMemory(OggBuf.get(), 255);

		if(m_i3Pack->PackRead((void **)&OggBuf, 255, itf->Offset) != 0)
			THROW("Could not read OGG header.");

		OggHeader *pOgg = (OggHeader *)OggBuf.get();

		if(pOgg->Signature != 0x5367674F)
			THROW("Invalid OGG file format on i3Pack.");

		CString s, sc(": ");
		m_stcFilename.SetWindowText(sc + itf->Filename);
		s.Format("%d Hz", pOgg->Segments[0].SampleRate);
		m_stcSampleRate.SetWindowText(sc + s);
		m_stcBitrate.SetWindowText(sc + FormatSize(pOgg->Segments[0].BitrateNom));
		m_stcChannels.SetWindowText(sc + (pOgg->Segments[0].Channels == 1 ? "Mono" : "Stereo"));
		m_stcCodec.SetWindowText(sc + CString(pOgg->Segments[0].Signature));

		EnableDisableStaticInfo1(TRUE);
		m_btnBrowse2.EnableWindow(TRUE);
		m_btnChange.EnableWindow(FALSE);

	}
	catch(char *e) 
	{
		EnableDisableStaticInfo1(FALSE);
		EnableDisableStaticInfo2(FALSE);
		
		EmptyStaticInfo1();
		EmptyStaticInfo2();
		
		m_btnBrowse2.EnableWindow(FALSE);
		m_btnChange.EnableWindow(FALSE);

		if(m_i3Pack->IsFileOpened())
			m_i3Pack->Close();

		m_i3Pack->ClearBufferList();

		AfxMessageBox(e);
		return;
	}
}

CString CBGMChangerDlg::FormatSize(ULONG ulSize) {
	static const ULONG ulKB = 1024;
	static const ULONG ulMB = 1024 * ulKB;
	static const ULONG ulGB = 1024 * ulMB;

	CString ret;
	if(ulSize < ulKB) {
		ret.Format("%d bps");
	} else {
		if(ulSize < ulMB) {
			ret.Format("%d kbps", ulSize / ulKB);
		} else {
			ret.Format("%d mbps", ulSize / ulMB);
		}
	}
	return ret;
}

void CBGMChangerDlg::EmptyStaticInfo1() {
	CString sc(": ");
	m_stcFilename.SetWindowText(sc);
	m_stcSampleRate.SetWindowText(sc);
	m_stcBitrate.SetWindowText(sc);
	m_stcChannels.SetWindowText(sc);
	m_stcCodec.SetWindowText(sc);
}

void CBGMChangerDlg::EmptyStaticInfo2() {
	CString sc(": ");
	m_stcFilename2.SetWindowText(sc);
	m_stcSampleRate2.SetWindowText(sc);
	m_stcBitrate2.SetWindowText(sc);
	m_stcChannels2.SetWindowText(sc);
	m_stcCodec2.SetWindowText(sc);
}


void CBGMChangerDlg::EnableDisableStaticInfo1(BOOL bState) {
	m_stcFilename.EnableWindow(bState);
	m_stcSampleRate.EnableWindow(bState);
	m_stcBitrate.EnableWindow(bState);
	m_stcChannels.EnableWindow(bState);
	m_stcCodec.EnableWindow(bState);

	GetDlgItem(IDC_STC_INFOFILE)->EnableWindow(bState);
	GetDlgItem(IDC_STC_INFOSR)->EnableWindow(bState);
	GetDlgItem(IDC_STC_INFOBR)->EnableWindow(bState);
	GetDlgItem(IDC_STC_INFOCHAN)->EnableWindow(bState);
	GetDlgItem(IDC_STC_INFOCODEC)->EnableWindow(bState);
	GetDlgItem(IDC_STC_GROUP01)->EnableWindow(bState);

	if(!bState) m_editPath.SetWindowText(0);
	m_editPath.EnableWindow(bState);
}

void CBGMChangerDlg::EnableDisableStaticInfo2(BOOL bState) {
	m_stcFilename2.EnableWindow(bState);
	m_stcSampleRate2.EnableWindow(bState);
	m_stcBitrate2.EnableWindow(bState);
	m_stcChannels2.EnableWindow(bState);
	m_stcCodec2.EnableWindow(bState);

	GetDlgItem(IDC_STC_INFOFILE2)->EnableWindow(bState);
	GetDlgItem(IDC_STC_INFOSR2)->EnableWindow(bState);
	GetDlgItem(IDC_STC_INFOBR2)->EnableWindow(bState);
	GetDlgItem(IDC_STC_INFOCHAN2)->EnableWindow(bState);
	GetDlgItem(IDC_STC_INFOCODEC2)->EnableWindow(bState);
	GetDlgItem(IDC_STC_GROUP02)->EnableWindow(bState);

	if(!bState) m_editPath2.SetWindowText(0);
	m_editPath2.EnableWindow(bState);
}

void CBGMChangerDlg::OnBnClickedBtnbrowse2()
{
	try
	{
		CFileDialog fileDlg(TRUE, "ogg", NULL, OFN_FILEMUSTEXIST, "OGG Audio File (*.ogg)|*.ogg||");
		if(fileDlg.DoModal() != IDOK)
			return;

		CString csFile = fileDlg.GetPathName();

		m_csOggPath = csFile;

		if(!IsFileExists(csFile))
			THROW("File is not exists.");

		m_editPath2.SetWindowText(csFile);

		if(m_pOGGFile->m_hFile != INVALID_HANDLE_VALUE)
			m_pOGGFile->Close();

		if(m_pOGGFile->Open(csFile, CFile::modeRead) == FALSE)
			THROW("Could not open your OGG file.");

		CFileStatus fs;
		if(!m_pOGGFile->GetStatus(fs))
			THROW("Could not get file status.");

		ULONG ulFileSize = (ULONG)fs.m_size;

		SAFEBUFFER(OggBuf, ulFileSize);
		ZeroMemory(OggBuf.get(), ulFileSize);

		if(m_pOGGFile->Read(OggBuf.get(), ulFileSize) != ulFileSize)
			THROW("Could not read OGG file because return filesize are not equals.");

		OggHeader *pOgg = (OggHeader *)OggBuf.get();

		if(pOgg->Signature != 0x5367674F)
			THROW("Invalid OGG file format.");

		CString s, sc(": ");

		CString strippedPath = CString(PathFindFileName(csFile));

		const int LIMIT_TEXT = 35;
		if(strippedPath.GetLength() > LIMIT_TEXT) {
			strippedPath = strippedPath.Mid(0, LIMIT_TEXT);
			strippedPath += "...";
		}

		m_stcFilename2.SetWindowText(sc + strippedPath);
		s.Format("%d Hz", pOgg->Segments[0].SampleRate);
		m_stcSampleRate2.SetWindowText(sc + s);
		m_stcBitrate2.SetWindowText(sc + FormatSize(pOgg->Segments[0].BitrateNom));
		m_stcChannels2.SetWindowText(sc + (pOgg->Segments[0].Channels == 1 ? "Mono" : "Stereo"));
		m_stcCodec2.SetWindowText(sc + CString(pOgg->Segments[0].Signature));

		EnableDisableStaticInfo2(TRUE);
		m_btnChange.EnableWindow(TRUE);

		OggBuf.reset();

	}
	catch(char *e) {
		if(m_pOGGFile) {
			if(m_pOGGFile->m_hFile != INVALID_HANDLE_VALUE)
				m_pOGGFile->Close();
		}

		EmptyStaticInfo2();
		EnableDisableStaticInfo2(FALSE);
		m_btnChange.EnableWindow(FALSE);

		AfxMessageBox(e);
		return;
	}
}

BOOL CBGMChangerDlg::IsFileExists(CString csFile) {
	if(csFile.IsEmpty())
		return false;

	CFileStatus fs;
	return CFile::GetStatus(csFile, fs);
}

void CBGMChangerDlg::OnBnClickedBtnChange()
{
	vector<CSingleNode> *pvNodes = 0;
	vector<CHeaderNodeInfo> *pvDirs = 0;

	void *pwriteFile = 0;

	try
	{
		CString csBaseOggFN = PathFindFileName(m_csOggPath);
		PathRemoveExtension((LPSTR)csBaseOggFN.GetBuffer(0));

		CString csOpenedFN = PathFindFileName(m_csOpenedPath);
		CString csOpenedExt = PathFindExtension(csOpenedFN);
		PathRemoveExtension((LPSTR)csOpenedFN.GetBuffer(0));

		csBaseOggFN.Replace(' ', '_');

		CString csFmtSaveAs;
		csFmtSaveAs.Format("%s_[%s]%s", csOpenedFN, csBaseOggFN, csOpenedExt);

		CFileDialog fileDlg(FALSE, "i3Pack", csFmtSaveAs, OFN_OVERWRITEPROMPT, "i3Pack (*.i3Pack)|*.i3Pack||");
		if(fileDlg.DoModal() != IDOK)
			return;

		CString csSaveFile = fileDlg.GetPathName();

		CFile saveFile;
		if(saveFile.Open(csSaveFile, CFile::modeCreate | CFile::modeWrite) == FALSE)
			THROW("Could not create your modified file.");

		if(!m_pOGGFile)
			THROW("m_pOGGFile == NULL");

		// nodes
		pvNodes = m_i3Pack->GetNodes();

		vector<CSingleNode>::iterator it;
		it = std::find_if(pvNodes->begin(), pvNodes->end(), boost::BOOST_BIND(&CSingleNode::NodeName, _1) == "BackGround");

		if(it == pvNodes->end())
			THROW("Could not find required node.");

		vector<CNodeFileInfo>::iterator itf;
		itf = std::find_if(it->Files.begin(), it->Files.end(), boost::BOOST_BIND(&CNodeFileInfo::Filename, _1) == "Menu_BGM.ogg");
		if(itf == it->Files.end())
			THROW("Could not find required file on i3Pack");

		ULONG ulNodeIndex = it->Index;

		// dirs
		pvDirs = m_i3Pack->GetDirs();
		
		vector<CHeaderNodeInfo>::iterator itd;

		itd = std::find_if(pvDirs->begin(), pvDirs->end(), boost::BOOST_BIND(&CHeaderNodeInfo::Index, _1) == ulNodeIndex);

		if(itd == pvDirs->end())
			THROW("Could not find required directory index.");
		

		ULONG ulBaseDir = itd->BaseAddr;

		// malloc stuffs
		ULONG ulAllocSize1 = itf->Offset;
		pwriteFile = malloc(ulAllocSize1);
		if(!pwriteFile)
			THROW("Could not allocate buffers at first section.");

		SAFEBUFFER(pBuf1, ulAllocSize1);
		ZeroMemory(pBuf1.get(), ulAllocSize1);

		if(m_i3Pack->PackRead((void **)&pBuf1, ulAllocSize1) != 0) 
			THROW("Could not read first section of buffer.");

		if(memcpy_s(pwriteFile, ulAllocSize1, pBuf1.get(), ulAllocSize1) != 0)
			THROW("Could not copy first buffer to destination.");

		pBuf1.reset();

		CFileStatus fs;
		if(!m_pOGGFile->GetStatus(fs))
			THROW("Could not get file status.");

		ULONG ulOggFilesize = (ULONG)fs.m_size;
		
		SAFEBUFFER(pBuf2, ulOggFilesize);
		ZeroMemory(pBuf2.get(), ulOggFilesize);

		m_pOGGFile->Seek(0, CFile::begin);
		if(m_pOGGFile->Read(pBuf2.get(), ulOggFilesize) != ulOggFilesize)
			THROW("Could not read OGG file because return size are not equals.");

		void *pNewPtr = realloc(pwriteFile, ulAllocSize1+ulOggFilesize);
		if(!pNewPtr)
			THROW("Could not reallocate new size buffer at second section.");

		pwriteFile = pNewPtr;
		if(memcpy_s((void *)(ULONG(pwriteFile)+ulAllocSize1), ulOggFilesize, pBuf2.get(), ulOggFilesize) != 0)
			THROW("Could not copy second buffer to destination.");

		pBuf2.reset();

		ULONG ulPackSize = m_i3Pack->GetPackSize();
		if(ulPackSize == 0)
			THROW("Could not get i3Pack size.");

		ULONG ulOffTail = itf->Offset + itf->Size;
		ULONG ulSizeTail = ulPackSize - ulOffTail;

		SAFEBUFFER(pBuf3, ulSizeTail);
		ZeroMemory(pBuf3.get(), ulSizeTail);

		if(m_i3Pack->PackRead((void **)&pBuf3, ulSizeTail, ulOffTail) != 0) 
			THROW("Could not read OGG file because return size are not equals.");

		pNewPtr = realloc(pwriteFile, ulAllocSize1+ulOggFilesize+ulSizeTail);
		if(!pNewPtr)
			THROW("Could not reallocate new size buffer at third section.");

		pwriteFile = pNewPtr;

		if(memcpy_s((void *)(ULONG(pwriteFile)+ulAllocSize1+ulOggFilesize), ulSizeTail, pBuf3.get(), ulSizeTail) != 0)
			THROW("Could not copy third buffer to destination.");

		pBuf3.reset();

		ULONG ulTotalSize = ulAllocSize1+ulOggFilesize+ulSizeTail;

		// ===============================================================================================

 		CNodeInfo *pNode = (CNodeInfo *)(ULONG(pwriteFile) + itd->BaseAddr);

		CPackFileInfo *pPackInfo = (CPackFileInfo *)(ULONG(pwriteFile) + pNode->dwOffset + itf->RawOffset);

		ULONG ulPackFileSize = itf->Padded ? sizeof(CPackFileInfo) : sizeof(CPackFileInfo)-16;

		BitRotateDecrypt(pPackInfo, ulPackFileSize, 2);
		
		if(itf->Padded) {
			pPackInfo->SizeShift_2	= (WORD)(ulOggFilesize >> 0x10);
			pPackInfo->SizeOr_2		= (WORD)(((ulOggFilesize >> 0x10) << 0x10) ^ ulOggFilesize);
		} else {
			pPackInfo->SizeShift_1	= (WORD)(ulOggFilesize >> 0x10);
			pPackInfo->SizeOr_1		= (WORD)(((ulOggFilesize >> 0x10) << 0x10) ^ ulOggFilesize);
		}

		BitRotateEncrypt(pPackInfo, ulPackFileSize, 2);

		// ===============================================================================================

		// table patch

		for(int i=0; i < m_i3Pack->GetNodeSize(); i++) {
			CNodeInfo *pNode_ = new CNodeInfo;
			ULONG ulRO = 0;
			if(m_i3Pack->GetDirectoryInfoById(&pNode_, i, ulRO) == FALSE)
				THROW("Error while do GetDirectoryInfoById.");

			if(itd->Index != pNode_->dwIndex) {
				CNodeInfo *pNode2 = (CNodeInfo *)(ULONG(pwriteFile) + ulRO);
				ULONG ulFirstAllocSize = ulAllocSize1 + ulOggFilesize;

				ULONG v1 = pNode2->dwOffset - (ulAllocSize1 + itf->Size);

				pNode2->dwOffset = ulFirstAllocSize + v1;
			}

			delete pNode_;
		}

		// ===============================================================================================
		// signature
		CPackHeader *pHead = 0;
		if(m_i3Pack->GetHeaderInfo(&pHead) == FALSE)
			THROW("Could not get header.");
		
		memcpy_s((void *)(ULONG(pwriteFile) + (pHead->dwStringTableOff - 6)), 5, (PBYTE *)"\x0A\xB1\x74\xF4\x12", 5);

		// ===============================================================================================
		// write and save buffer to file

		saveFile.Seek(0, CFile::begin);
		saveFile.Write(pwriteFile, ulTotalSize);
		saveFile.Close();

		AfxMessageBox("Your modified BGM has been saved!", MB_ICONINFORMATION);

	}
	catch (char* e)
	{
		if(pvNodes)
			pvNodes = 0;

		if(pvDirs)
			pvDirs = 0;

		if(pwriteFile) {
			free(pwriteFile);
			pwriteFile = 0;
		}

		AfxMessageBox(e);
		return;
	}

	if(pvNodes)
		pvNodes = 0;

	if(pvDirs)
		pvDirs = 0;

	if(pwriteFile) {
		free(pwriteFile);
		pwriteFile = 0;
	}
}
