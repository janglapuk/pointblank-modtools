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


// i3PackToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "i3PackTool.h"
#include "i3PackToolDlg.h"
#include "Crypt.h"
#include "FolderDlg.h"
#include "CRC32.h"
#include <Shlwapi.h>
//#include <pespin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CRLF		"\r\n"

static UINT BASED_CODE indicators[] =
{
	ID_INDICATOR_FIRST,
	ID_INDICATOR_SECOND
};

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


// Ci3PackToolDlg dialog




Ci3PackToolDlg::Ci3PackToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ci3PackToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

Ci3PackToolDlg::~Ci3PackToolDlg()
{
	m_I3Pack.ClearBufferList();

	if(m_I3Pack.IsFileOpened())
		m_I3Pack.Close();

}


void Ci3PackToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREENODE, m_treeNode);
	DDX_Control(pDX, IDC_LISTCONTENT, m_listContent);
	DDX_Control(pDX, IDC_EDITPATH, m_editFilePath);
}

BEGIN_MESSAGE_MAP(Ci3PackToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_LISTCTL_PROPERTIES, &Ci3PackToolDlg::OnListctlProperties)
	ON_BN_CLICKED(IDC_BTNBROWSE, &Ci3PackToolDlg::OnBnClickedBtnbrowse)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREENODE, &Ci3PackToolDlg::OnTvnSelchangedTreenode)
	ON_COMMAND(ID_LISTCTL_DUMP, &Ci3PackToolDlg::OnListctlDump)
	ON_COMMAND(ID_LISTCTL_DUMPALLFILES, &Ci3PackToolDlg::OnListctlDumpallfiles)
	ON_COMMAND(ID_LISTCTL_COPYDETAILS, &Ci3PackToolDlg::OnListctlCopyDetails)
	ON_COMMAND(ID_LISTCTL_REPLACEFILEWITH, &Ci3PackToolDlg::OnListctlReplaceFileWith)
END_MESSAGE_MAP()


// Ci3PackToolDlg message handlers

BOOL Ci3PackToolDlg::OnInitDialog()
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

	InitTreeNodeControl();
	InitListControl();

	// status bar
	{
		m_StatusBar.Create(this);
		m_StatusBar.SetIndicators(indicators, 2);

		CRect rect;
		GetClientRect(&rect);

		//Size the two panes
		m_StatusBar.SetPaneInfo(0, ID_INDICATOR_FIRST, SBPS_NORMAL, rect.Width()-110);
		m_StatusBar.SetPaneInfo(1, ID_INDICATOR_SECOND, SBPS_STRETCH, 0);

		//This is where we actually draw it on the screen
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_SECOND);

		SetSBFileCount(0);

//		CLEAR_START;

		CString csXor;
		NotXorBit((PBYTE)"\xAC\xAC\xAC\xED\xEE\xF9\xE6\xED\xEA\xED\xFE\xAC\x25\xAC\xBE\xBC\xBD\xBF\x8C" /*   abujafar © 2013*/, 19, csXor, 0x73);
		m_StatusBar.SetPaneText(1, csXor);

		NotXorBit((PBYTE)"\xE5\xBF\xDC\xED\xEF\xE7\xD8\xE3\xE3\xE0\xAC\xA4\xED\xE0\xFC\xE4\xED\xA1\xBE\xBC\xBD\xBF\xBC\xBF\xBE\xB5\xA5\x8C" /*i3PackTool (alpha-20130324)*/, 28, csXor, 0x73);
		SetWindowText(csXor);
		//m_StatusBar.SetPaneStyle(1, m_StatusBar.GetPaneStyle(1) | CBRS_ALIGN_RIGHT);

//		CLEAR_END;
	}
	
	m_pSelectedNode = 0;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ci3PackToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Ci3PackToolDlg::OnPaint()
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
HCURSOR Ci3PackToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ci3PackToolDlg::OnContextMenu(CWnd* pWnd, CPoint point) {
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LISTCONTENT);
	if ((CWnd*)pList == pWnd)
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU1));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while(pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
								point.x, 
								point.y,
								pWndPopupOwner);
	}
}

void Ci3PackToolDlg::InitListControl() {
	m_listContent.SetExtendedStyle(LVS_EX_FULLROWSELECT /*| LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE*/);
	m_listContent.InsertColumn(0, TEXT("Filename"), LVCFMT_CENTER, 300);
	m_listContent.InsertColumn(1, TEXT("Offset"), LVCFMT_CENTER, 90);
	m_listContent.InsertColumn(2, TEXT("Size"), LVCFMT_CENTER, 90);
	m_listContent.InsertColumn(3, TEXT("CRC32"), LVCFMT_CENTER, 90);

// 	// dummy data
// 	LVITEM lvi;
// 	CString strItem;
// 	void *lpDummy = malloc(50*4);
// 
// 	memcpy(lpDummy, (void *)(ULONG)(0x400000), 50*4);
// 
// 	for(int i=0; i<50; i++) {
// 		// subitem 0
// 		lvi.mask = LVIF_TEXT;
// 		lvi.iItem = i;
// 		lvi.iSubItem = 0;
// 		strItem.Format("file%02d.txt", i+1);
// 		lvi.pszText = (LPTSTR)(LPCTSTR)strItem;
// 		m_listContent.InsertItem(&lvi);
// 
// 		lvi.iSubItem = 1;
// 		strItem.Format("%X", 100*(i+1));
// 		lvi.pszText = (LPTSTR)(LPCTSTR)strItem;
// 		m_listContent.SetItem(&lvi);
// 
// 		lvi.iSubItem = 2;
// 		strItem.Format("%d", 1000*(i+1));
// 		lvi.pszText = (LPTSTR)(LPCTSTR)strItem;
// 		m_listContent.SetItem(&lvi);
// 
// 
// 		DWORD dwCRC32 = 0, dwOff = 0;
// 		dwOff = ULONG(lpDummy)+(i*4);
// 		CCrc32Static::Crc32((BYTE*)dwOff, 64, dwCRC32);
// 		
// 		lvi.iSubItem = 3;
// 		strItem.Format("%08X", dwCRC32);
// 		lvi.pszText = (LPTSTR)(LPCTSTR)strItem;
// 		m_listContent.SetItem(&lvi);
// 	}
// 	free(lpDummy);
}

void Ci3PackToolDlg::InitTreeNodeControl() {

}

vector<CString> Ci3PackToolDlg::split(CString& str, char* pszSeps) {
	vector<CString> result;
	char* psz = strdup ( str.GetBuffer(0));
	char* token = NULL;
	token  = strtok ( psz, pszSeps);
	while ( token) {
		CString sub = token;
		result.push_back ( sub);
		token = strtok( NULL, pszSeps);
	} 
	free (psz);
	return result;
};

void Ci3PackToolDlg::OnBnClickedBtnbrowse()
{
	DoOpeni3PackFile();
}

void Ci3PackToolDlg::DoOpeni3PackFile( CString csFile/*=0*/ )
{
	
	try
	{
		if(csFile.IsEmpty()) {
			CFileDialog fileDlg(TRUE, "i3Pack", "", OFN_READONLY, "i3Pack File (*.i3Pack)|*.i3Pack||");
			if(fileDlg.DoModal() != IDOK)
				return;

			m_strOpenedPath = fileDlg.GetPathName();
		} else {
			m_strOpenedPath = csFile;
		}

		m_vecNodes.clear();
		m_listContent.DeleteAllItems();
		m_treeNode.DeleteAllItems();
		SetSBFileCount(0);
	
		m_editFilePath.SetWindowText(m_strOpenedPath);

		m_I3Pack.ClearBufferList();
		if(m_I3Pack.IsFileOpened())
			m_I3Pack.Close();

		if(m_I3Pack.Open(m_strOpenedPath) != 0)
			THROW("Could not open your i3Pack file.");

		m_treeNode.DeleteAllItems();

		vector<CSingleNode> *pNodes = m_I3Pack.GetNodes();

		HTREEITEM hti;
		ULONG ulTreeIndex = 0;

		for(vector<CSingleNode>::reverse_iterator rit=pNodes->rbegin(); rit != pNodes->rend(); ++rit) {
			ulTreeIndex = rit->Index;
		
			CString strNodeName = rit->NodeName;
			if(rit->IsRoot()) {
				hti = m_treeNode.InsertItem(strNodeName, TVI_ROOT);
			} else {
				if(rit->IsLeaf()) {
					HTREEITEM _hti = m_treeNode.InsertItem(strNodeName, hti, TVI_LAST);
					if(rit->FileCount)
						m_treeNode.SetItemState(_hti, TVIS_BOLD, TVIS_BOLD);
				} else {
					hti = m_treeNode.InsertItem(strNodeName, hti, TVI_LAST);
					if(rit->FileCount)
						m_treeNode.SetItemState(hti, TVIS_BOLD, TVIS_BOLD);
				}
			}

		}

#pragma region HIDE
		//m_I3Pack.Close();
	
// 		if(m_i3File.Open(m_strOpenedPath, CFile::modeRead) == FALSE)
// 			THROW("Cannot open file.");
// 	
// 		m_i3File.Seek(0, CFile::begin);
// 	
// 		//boost::shared_ptr<BYTE> pucHeader((BYTE*)malloc(0xB8), free);
// 		SAFEBUFFER(pucHeader, 0xB8);
// 
// // 		CString msg;
// // 		msg.Format("pucHeader: %X", pucHeader.get());
// // 		AfxMessageBox(msg);
// 
// 		ZeroMemory(pucHeader.get(), 0xB8);
// 	
// 		if(m_i3File.Read(pucHeader.get(), 0xB8) != 0xB8)
// 			THROW("Read header return are not equals.");
// 	
// 		CPackHeader *pPackHeader = (CPackHeader *)pucHeader.get();
// 		
// 		if(CString(pPackHeader->szHeaderID) != CString("I3R2"))
// 			THROW("Header file identifier are not equals.");
// 	
// 		SAFEBUFFER(pStringTable, pPackHeader->dwStringTableSize);
// 	
// 		ZeroMemory(pStringTable.get(), pPackHeader->dwStringTableSize);
// 	
// 		m_i3File.Seek(pPackHeader->dwStringTableOff, CFile::begin);
// 		if(m_i3File.Read(pStringTable.get(), pPackHeader->dwStringTableSize) != pPackHeader->dwStringTableSize) {
// 			THROW("String table header return are not equals.");
// 		}
// 
// 		vector<CString> vecStringTable = split(CString((char *)pStringTable.get()), "\r\n");
// 		vecStringTable.erase(std::remove(vecStringTable.begin(), vecStringTable.end(), CString("i3PackNode")));
// 		// ---------------------------------------------------------------------------
// 
// 		m_pNodeInfoBuffer = boost::shared_ptr<BYTE>((BYTE *)malloc(pPackHeader->dwDirTableSize), free);
// 	
// 		m_i3File.Seek(pPackHeader->dwDirTableOff, CFile::begin);
// 		if(m_i3File.Read(m_pNodeInfoBuffer.get(), pPackHeader->dwDirTableSize) != pPackHeader->dwDirTableSize) {
// 			THROW("Read node info return are not equals.");
// 		}
// 	
// 		for(int i=0; i < (int)pPackHeader->dwNodeCount; i++) {
// 			CNodeInfo *pNode = 0;
// 			if((pNode = GetNodeInfoById(m_pNodeInfoBuffer.get(), pPackHeader->dwNodeCount, i))) {
// 
// 				SAFEBUFFER(pNodeBuffer, pNode->dwSize);
// 				ZeroMemory(pNodeBuffer.get(), pNode->dwSize);
// 
// 				m_i3File.Seek(pNode->dwOffset, CFile::begin);
// 
// 				if(m_i3File.Read(pNodeBuffer.get(), pNode->dwSize) != pNode->dwSize)
// 					THROW("Could not read node information to buffer.");
// 
// 				int nStrNodeLen = *(BYTE*)pNodeBuffer.get();
// 				ULONG ulFileCountOff = (pNode->dwOffset + nStrNodeLen + 1 + NODE_PADDING);
// 
// 				ULONG ulPad = *(ULONG*)(pNodeBuffer.get() + 1 + nStrNodeLen + NODE_PADDING);
// 				if(ulPad > 0 && ulPad < pPackHeader->dwNodeCount)
// 					ulFileCountOff += 4;
// 
// 				ULONG ulPad2 = *(ULONG*)(pNodeBuffer.get() + 1 + nStrNodeLen + NODE_PADDING + 4);
// 				//if(ulPad2 > ulPad && ulPad2 < pPackHeader->dwNodeCount)
// 				if(ulPad2 == ulPad+1)
// 					ulFileCountOff += 4;
// 
// 				BYTE ucNodeCryptSec[8];
// 				ZeroMemory(&ucNodeCryptSec, 8);
// 				
// 				m_i3File.Seek(ulFileCountOff, CFile::begin);
// 				if(m_i3File.Read(&ucNodeCryptSec, 8) != 8)
// 					THROW("Could not read node crypted section.");
// 
// 				BitRotateDecrypt(&ucNodeCryptSec, sizeof(sNodeFileInfo), 3);
// 
// 				sNodeFileInfo *sNodeCrypt = (sNodeFileInfo *)ucNodeCryptSec;
// 
// 				CSingleNode Node(vecStringTable[i], pNode->dwIndex, pNode->dwOffset, pNode->dwSize, sNodeCrypt->FileCount);
// 
// 				bool bPadded = false;
// 
// 				for(int i=0; i < (int)sNodeCrypt->FileCount; i++) {
// 					ULONG ulFileInfoBase = ulFileCountOff + 8 + (i * (bPadded ? sizeof(CPackFileInfo) : sizeof(CPackFileInfo)-16));
// 
// 					ULONG ulSize = sizeof(CPackFileInfo);
// 					
// 					SAFEBUFFER(pBuffer, sizeof(CPackFileInfo));
// 					ZeroMemory(pBuffer.get(), sizeof(CPackFileInfo));
// 
// 					m_i3File.Seek(ulFileInfoBase, CFile::begin);
// 					if(m_i3File.Read(pBuffer.get(), sizeof(CPackFileInfo)) != sizeof(CPackFileInfo))
// 						THROW("Could not read file info.\nSome files cannot to be read properly because the application has a bug.\nI will fix it later.");
// 
// 					CPackFileInfo *FileInfo = (CPackFileInfo *)pBuffer.get();
// 
// 					if(FileInfo->Ended == 0x01000000)
// 						bPadded = true;
// 					else
// 						bPadded = false;
// 
// 					ULONG ulPackSize = (bPadded ? sizeof(CPackFileInfo) : sizeof(CPackFileInfo)-16);
// 
// 					BitRotateDecrypt(FileInfo, ulPackSize, 2);
// 
// 					ULONG ulPushOffset	= bPadded ? (FileInfo->OffsShift_2 << 0x10) | FileInfo->OffsOr_2 : (FileInfo->OffsShift_1 << 0x10) | FileInfo->OffsOr_1;
// 					ULONG ulPushSize	= bPadded ? (FileInfo->SizeShift_2 << 0x10) | FileInfo->SizeOr_2 : (FileInfo->SizeShift_1 << 0x10) | FileInfo->SizeOr_1;
// 
// 					Node.Files.push_back(CNodeFileInfo(FileInfo->szFilename, 
// 													   ulPushOffset,
// 													   ulPushSize
// 													  ));
// 				}
// 
// 				m_vecNodes.push_back(Node);
// 			}
// 		}
// 	
// 		
// 		// ---------------------------------------------------------------------------
// 		reverse(m_vecNodes.begin(), m_vecNodes.end());
// 		m_treeNode.DeleteAllItems();
// 	
// 		HTREEITEM hti;
// 		for(int i=0; i < (int)m_vecNodes.size(); i++) {
// 			CString strNodeName = m_vecNodes[i].NodeName;
// 
// // 			if(m_vecNodes[i].FileCount) {
// // 				strNodeName.Format("%s (%d)", m_vecNodes[i].NodeName, m_vecNodes[i].FileCount);
// // 			}
// 
// 			if(i == 0)
// 				hti = m_treeNode.InsertItem(strNodeName, TVI_ROOT);
// 			else
// 				hti = m_treeNode.InsertItem(strNodeName, hti, TVI_LAST);
// 
// 			if(m_vecNodes[i].FileCount)
// 				m_treeNode.SetItemState(hti, TVIS_BOLD, TVIS_BOLD);
// 		}
// 	
#pragma endregion

		ExpandAllTree(&m_treeNode);
	}
	catch(char *e)
	{
		m_I3Pack.ClearBufferList();
		if(m_I3Pack.IsFileOpened())
			m_I3Pack.Close();

		AfxMessageBox(e);
		return;
	}
}

void Ci3PackToolDlg::ExpandAllTree(CTreeCtrl *pCtl) {
	//expand all
	HTREEITEM hItem;
	hItem = pCtl->GetFirstVisibleItem();
	while(hItem != NULL)
	{
		pCtl->Expand(hItem,TVE_EXPAND);
		hItem = pCtl->GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}
}
void Ci3PackToolDlg::OnTvnSelchangedTreenode(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
		HTREEITEM hItem = m_treeNode.GetSelectedItem();
	
		if(hItem) {
			m_listContent.DeleteAllItems();
			SetSBFileCount(0);
	
			if((m_treeNode.GetItemState(hItem, TVIS_BOLD) & TVIS_BOLD) && m_I3Pack.IsFileOpened()) {
				
				CString nodeName = m_treeNode.GetItemText(hItem);
				
				vector<CSingleNode> *pNodes = m_I3Pack.GetNodes();
				vector<CSingleNode>::iterator it = std::find_if(pNodes->begin(), pNodes->end(), boost::BOOST_BIND(&CSingleNode::NodeName, _1) == nodeName);
	
				if(it != pNodes->end()) {
					CString itemList;
					int listIt = 0;

					m_pSelectedNode = &(*it);
	
// 					if(m_i3File.Open(m_strOpenedPath, CFile::modeRead) == FALSE)
// 						THROW("Cannot open file.");
	
					CRC32 crc32;

					for(vector<CNodeFileInfo>::iterator fit=it->Files.begin(); fit != it->Files.end(); ++fit) {
						m_listContent.InsertItem(listIt, fit->Filename);
	
						itemList.Format("%X", fit->Offset);
						m_listContent.SetItem(listIt, 1, LVIF_TEXT, itemList, 0, 0, 0, 0);
	
						itemList.Format("%X", fit->Size);
						m_listContent.SetItem(listIt, 2, LVIF_TEXT, itemList, 0, 0, 0, 0);
	
						SAFEBUFFER(pData, fit->Size);
						
//						m_i3File.Seek(fit->Offset, CFile::begin);
	
						int nCRC = 0;

						// ##CRC32##
// 						if(m_i3File.Read(pData.get(), fit->Size) == fit->Size) {
// 							nCRC = crc32.Get_CRC(pData.get(), fit->Size);
// 						}
						if(m_I3Pack.PackRead((void **)&pData, fit->Size, fit->Offset) == 0) {
							nCRC = crc32.Get_CRC(pData.get(), fit->Size);
						}

						pData.reset();
	
						itemList.Format("%08X", nCRC);
						m_listContent.SetItem(listIt, 3, LVIF_TEXT, itemList, 0, 0, 0, 0);
	
						UpdateData(FALSE);

						listIt++;
	
					}

					SetSBFileCount(it->Files.size());
//					m_i3File.Close();
				}
			}
		}
	}

	catch (char *e)
	{
		AfxMessageBox(e);
		m_i3File.Close();
	}

	*pResult = 0;
}

// CNodeInfo *Ci3PackToolDlg::GetNodeInfoById( void *lpBase, int nSize, int nIndex )
// {
// 	if(lpBase && nSize) {
// 		if(nIndex < nSize) {
// 			return (CNodeInfo *)(ULONG(lpBase) + (sizeof(CNodeInfo) * nIndex));
// 		}
// 	}
// 	return 0;
// }

void Ci3PackToolDlg::SetSBFileCount(int nCount) {
	CString s;
	s.Format("%d total files", nCount);
	m_StatusBar.SetPaneText(0, s);
}

void Ci3PackToolDlg::OnListctlDump()
{
	try 
	{
		int nListRow = m_listContent.GetSelectionMark();
		if(nListRow < 0) 
			return;

		if(m_pSelectedNode->Files.size() == 0)
			return;
			//THROW("Information on this file doesn't seem right. Dump aborted.");
	
		CString strFile = m_listContent.GetItemText(nListRow, 0);
		if(strFile.IsEmpty())
			return;
	
		vector<CNodeFileInfo>::iterator it;
	
		it = std::find_if(m_pSelectedNode->Files.begin(), m_pSelectedNode->Files.end(), boost::BOOST_BIND(&CNodeFileInfo::Filename, _1) == strFile);
	
		if(it != m_pSelectedNode->Files.end()) {

			if(it->Offset == 0 || it->Size == 0)
				THROW("Information on this file doesn't seem right. Dump aborted.");

			SAFEBUFFER(pBuffer, it->Size);

			if(m_I3Pack.PackRead((void **)&pBuffer, it->Size, it->Offset) != 0)
 				THROW("Could not read original file to dump this selection.");

			CString fileExt = PathFindExtension(strFile);
			fileExt.TrimLeft('.');

			CString strFilter;
			strFilter.Format("%s Files (*.%s)|*.%s||", fileExt, fileExt, fileExt);

			CFileDialog DumpFile(FALSE, fileExt, strFile, OFN_OVERWRITEPROMPT, strFilter);

			if(DumpFile.DoModal() == IDOK) {
				CString SavePath = DumpFile.GetPathName();
				
				CFile WriteDump;
				if(WriteDump.Open(SavePath, CFile::modeCreate | CFile::modeWrite) == FALSE)
					THROW("Could not create dump file.");

				WriteDump.Seek(0, CFile::begin);
				WriteDump.Write(pBuffer.get(), it->Size);
				WriteDump.Close();

				CString s;
				s.Format("File '%s' has been dumped successfully.", strFile);
				AfxMessageBox(s, MB_ICONINFORMATION);
			}
		}
	}
	catch(char *e)
	{
		AfxMessageBox(e);
	}
}

void Ci3PackToolDlg::OnListctlDumpallfiles()
{
	try
	{
		int nListRow = m_listContent.GetSelectionMark();
		if(nListRow < 0) 
			return;
	
		if(m_pSelectedNode->Files.size() == 0)
			return;
			//THROW("Information on this file doesn't seem right. Dump aborted.");

		CString DumpPath = m_strOpenedPath;
		PathRemoveFileSpec(DumpPath.GetBuffer(0));

		CString DumpBaseName(PathFindFileName(m_strOpenedPath));
	
		CFolderDialog DumpFolder("Select a directory to dump all files...", "", this, BIF_RETURNONLYFSDIRS | BIF_USENEWUI);
		DumpFolder.SetSelectedFolder(DumpPath);
	
		if(DumpFolder.DoModal() == IDOK) {
			CString FolderPath, SavedPath;

			FolderPath = CString(DumpFolder.GetFolderPath());

			CString DumpBaseFullName = CString(FolderPath + "\\_" + DumpBaseName);
			if(!PathFileExists(DumpBaseFullName)) {
				if(!CreateDirectory(DumpBaseFullName, NULL)) {
					THROW("Could not create dump base directory.");
				}
			}

			for(vector<CNodeFileInfo>::iterator it = m_pSelectedNode->Files.begin(); 
				it != m_pSelectedNode->Files.end(); 
				++it) 
			{
				if(it->Offset == 0 || it->Size == 0) {
					CString s;
					s.Format("Information on file '%s' doesn't seem right. Dump skipped on this file.", it->Filename);
					AfxMessageBox(s);
					continue;
				}

				SavedPath = FolderPath + "\\_" + DumpBaseName + "\\" + it->Filename;

				CFile DumpFile;
				if(DumpFile.Open(SavedPath, CFile::modeCreate | CFile::modeWrite) == FALSE) {
					CString s;
					s.Format("Error on creating dump file.\nPath: %s", SavedPath);
					AfxMessageBox(s);
					continue;
				}
				
				// read offset
				SAFEBUFFER(pBuffer, it->Size);

// 				m_i3File.Seek(it->Offset, CFile::begin);
// 				if(m_i3File.Read(pBuffer.get(), it->Size) != it->Size) {
// 					DumpFile.Close();
// 
// 					CString s;
// 					s.Format("Error on reading target file into buffer.\nPath: %s", SavedPath);
// 					AfxMessageBox(s);
// 					continue;
// 				}
				if(m_I3Pack.PackRead((void **)&pBuffer, it->Size, it->Offset) != 0) {
					DumpFile.Close();
					CString s;
					s.Format("Error on reading target file into buffer.\nPath: %s", SavedPath);
					AfxMessageBox(s);
					continue;
				}

				// write buffer
				DumpFile.Seek(0, CFile::begin);
				DumpFile.Write(pBuffer.get(), it->Size);

				// release buffer
				pBuffer.reset();
				DumpFile.Close();
			}
			AfxMessageBox("All files has been dumped. Enjoy!", MB_ICONINFORMATION);
		}
	}
	catch (char *e)
	{
		AfxMessageBox(e);
	}

	
}

void Ci3PackToolDlg::OnListctlProperties()
{
	int nListRow = m_listContent.GetSelectionMark();
	if(nListRow < 0) 
		return;

	CString msg;
	msg.Format("File\t: %s\nNode\t: %s\nIndex\t: %d\nFilename\t: %s\nOffset\t: %s\nSize\t: %s\nCRC32\t: %s", m_strOpenedPath, m_pSelectedNode->NodeName, nListRow, m_listContent.GetItemText(nListRow, 0), m_listContent.GetItemText(nListRow, 1), m_listContent.GetItemText(nListRow, 2), m_listContent.GetItemText(nListRow, 3));

	AfxMessageBox(msg, MB_ICONINFORMATION);
}

BOOL Ci3PackToolDlg::SetClipboardText( CString &szData )
{
	errno_t err = 0;
	HGLOBAL h;
	h=GlobalAlloc(GMEM_MOVEABLE, szData.GetLength());
	err = memcpy_s(GlobalLock(h), szData.GetLength(), szData.GetBuffer(), szData.GetLength());
	szData.ReleaseBuffer();
	GlobalUnlock(h);
	::OpenClipboard (NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, h);
	CloseClipboard();

	return FALSE == err;
}

void Ci3PackToolDlg::OnListctlCopyDetails()
{
	int nListRow = m_listContent.GetSelectionMark();
	if(nListRow < 0) 
		return;

	CString msg;
	msg.Format("Path\t\t: %s\r\n"
			   "Node name\t: %s\r\n"
			   "Index\t\t: %d\r\n"
			   "File name\t: %s\r\n"
			   "Offset\t\t: %s\r\n"
			   "Size\t\t: %s\r\n"
			   "CRC32\t\t: %s\r\n",
			   m_strOpenedPath,
			   m_pSelectedNode->NodeName,
			   nListRow, 
			   m_listContent.GetItemText(nListRow, 0), 
			   m_listContent.GetItemText(nListRow, 1), 
			   m_listContent.GetItemText(nListRow, 2), 
			   m_listContent.GetItemText(nListRow, 3)
			   );
	
	if(SetClipboardText(msg))
		AfxMessageBox("This information has copied to your clipboard.", MB_ICONINFORMATION);
}

void Ci3PackToolDlg::OnListctlReplaceFileWith()
{
	try {
		int nListRow = m_listContent.GetSelectionMark();
		if(nListRow < 0) 
			return;

		CString csFilename  = m_listContent.GetItemText(nListRow, 0);
		CString csNodeName  = m_pSelectedNode->NodeName;
		CString csFileExt	= PathFindExtension(csFilename);

		if(csFileExt.GetAt(0) == '.') {
			csFileExt.TrimLeft('.');
		}

		CString csFilter;
		csFilter.Format("%s File (*.%s)|*.%s||", csFilename, csFileExt, csFileExt);

		CFileDialog fileDlg(TRUE, csFileExt, csFilename, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, csFilter);

		if(fileDlg.DoModal() != IDOK)
			return;

		CString csOpenedFile = fileDlg.GetPathName();
		if(IsFileExists(csOpenedFile) == FALSE)
			THROW("File is not exists!");

		CFile fileOpen;
		if(fileOpen.Open(csOpenedFile, CFile::modeRead) == FALSE)
			THROW("Could not open target file.");

		CFileStatus fs;
		if(fileOpen.GetStatus(fs) == FALSE)
			THROW("Could not get status of target file.");

		ULONG ulFilesize = ULONG(fs.m_size);

		SAFEBUFFER(pBuffer, ulFilesize);
		ZeroMemory(pBuffer.get(), ulFilesize);

		if(fileOpen.Read(pBuffer.get(), ulFilesize) != ulFilesize)
			THROW("Could not read target file.");			

		if(m_I3Pack.ChangeFile(csNodeName, csFilename, (void *)pBuffer.get(), ulFilesize) == FALSE)
			THROW("Could not replace this file.");

		pBuffer.reset();
		fileOpen.Close();

		CString s;
		s.Format("Your modified file has been saved into new location: \n\n>>> %s <<<\n\nDo you want to open your modified i3Pack right now?", m_I3Pack.m_csReplacedFile);
		if(AfxMessageBox(s, MB_ICONQUESTION | MB_YESNO) == IDYES) {
			DoOpeni3PackFile(m_I3Pack.m_csReplacedFile);
		}

	} catch(char *e) {
		AfxMessageBox(e);
	}

}

BOOL Ci3PackToolDlg::IsFileExists(CString csFile) {
	CFileStatus fs;
	return CFile::GetStatus(csFile, fs);
}