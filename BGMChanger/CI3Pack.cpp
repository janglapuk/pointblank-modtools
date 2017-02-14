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


#include "stdafx.h"
#include "CI3Pack.h"
#include "../i3PackTool/Crypt.h"
#include <boost/bind.hpp>
#include <Shlwapi.h>


CI3Pack::CI3Pack()
{
	m_lpErr			= NULL;
	m_bIsFileOpen	= FALSE;

	m_pvPackNodes	= new vector<CSingleNode>;

	m_lpFile		= new CFile;

	m_pvHeaderDirInfo	= new vector<CHeaderNodeInfo>;
}

CI3Pack::~CI3Pack()
{
	m_HeaderBuffer.reset();

	if(m_lpFile) {
		if(m_lpFile->m_hFile != INVALID_HANDLE_VALUE)
			m_lpFile->Close();
		delete m_lpFile;
		m_lpFile = 0;
	}

	if(!m_pvPackNodes->empty()) {
		delete m_pvPackNodes;
	}

	if(!m_pvHeaderDirInfo->empty()) {
		delete m_pvHeaderDirInfo;
	}
}

int CI3Pack::Open( CString csFile )
{
	try
	{
		if(!m_lpFile)
			m_lpFile = new CFile;

		if(!m_lpFile->Open(csFile, CFile::modeRead))
			THROW(1);

		if(GetHeaderInfo(&m_lpHeader) == FALSE)
			THROW(2);

		if(CollectNodesInfo() == FALSE)
			THROW(3);

		m_csFilePath = csFile;

	}
	catch (CFileException* e)
	{
#ifdef _DEBUG
		AfxMessageBox(GetErrorMessage(e));
#endif
		return -1;
	}
	catch(int nError) 
	{
		return nError;
	}

	m_bIsFileOpen = TRUE;
	return 0;
}

CString CI3Pack::GetErrorMessage( CFileException *e )
{
	CString ret;
	e->GetErrorMessage(ret.GetBuffer(0), 255);
	return ret;
}

CString CI3Pack::GetErrorMessage( CException *e )
{
	CString ret;
	e->GetErrorMessage(ret.GetBuffer(0), 255);
	return ret;
}

BOOL CI3Pack::GetHeaderInfo(CPackHeader **pHeader)
{
	try 
	{
		if(!HasFileOpened())
			THROW("GetHeaderInfo: file has not opened.");
		
		UINT uiHeaderSize = sizeof CPackHeader;
		m_lpFile->Seek(0, CFile::begin);

		m_HeaderBuffer = ALLOCBUFFER(uiHeaderSize);
		ZeroMemory(m_HeaderBuffer.get(), uiHeaderSize);

		if(m_lpFile->Read(m_HeaderBuffer.get(), uiHeaderSize) != uiHeaderSize)
			THROW("GetHeaderInfo: read header return are not equals.");

		if(CString(((CPackHeader *)m_HeaderBuffer.get())->szHeaderID) != "I3R2")
			THROW("GetHeaderInfo: invalid i3Pack format.");

		*pHeader = (CPackHeader *)m_HeaderBuffer.get();
	}
	catch(char *e) {
#ifdef _DEBUG
		AfxMessageBox(e);
#endif
		return FALSE;
	}
	return TRUE;
}

BOOL CI3Pack::Close()
{
	m_lpFile->Close();
	return FALSE;
}

CString CI3Pack::GetFileID()
{
	if(m_lpHeader)
		return CString(m_lpHeader->szHeaderID);

	return 0;
}

int CI3Pack::GetNodeSize()
{
	if(m_lpHeader)
		return m_lpHeader->dwNodeSize;

	return -1;
}

ULONG CI3Pack::GetTableInfo( enum TableType type, enum TableInfo info )
{
	if(m_lpHeader) {
		if(type == TTString) {
			if(info == TIOffset) {
				return m_lpHeader->dwStringTableOff;
			} else {
				return m_lpHeader->dwStringTableSize;
			}
		}

		if(type == TTDirectory) {
			if(info == TIOffset) {
				return m_lpHeader->dwDirTableOff;
			} else {
				return m_lpHeader->dwDirTableSize;
			}
		}
	}

	return -1;
}

BOOL CI3Pack::GetNodeStringInfo( CString &str )
{
	try 
	{
		if(!m_lpFile)
			THROW("GetNodeStringInfo: file object not valid.");

		ULONG ulOffset	= GetTableInfo(TTString, TIOffset);
		ULONG ulSize	= GetTableInfo(TTString, TISize);
		m_lpFile->Seek(ulOffset, CFile::begin);

		SAFEBUFFER(lpString, ulSize);
		ZeroMemory(lpString.get(), ulSize);

		if(m_lpFile->Read(lpString.get(), ulSize) != ulSize)
			THROW("GetNodeStringInfo: string read return not equals.");

		str = (CString)lpString.get();
	}
	catch(char *e) {
#ifdef _DEBUG
		AfxMessageBox(e);
#endif
		return FALSE;
	}

	return TRUE;
}

BOOL CI3Pack::CollectNodesInfo()
{
	try 
	{
		CString nodeStr;
		if(GetNodeStringInfo(nodeStr) == FALSE)
			THROW("CollectNodesInfo: GetNodeStringInfo failed.");

		//vector<CString> vNodesName = SplitToVector(nodeStr, "\r\n");

// 		vNodesName.erase(std::remove(vNodesName.begin(), vNodesName.end(), CString("i3PackNode")));
// 		reverse(vNodesName.begin(), vNodesName.end());
		
		for(int i=0; i < GetNodeSize(); i++) {
			SAFEBUFFER(pBuffer, sizeof(CNodeInfo));

			ULONG ulDirOffs = 0;
			if(GetDirectoryInfoById((CNodeInfo **)&pBuffer, i, ulDirOffs) == FALSE)
				THROW("CollectNodesInfo: GetDirectoryInfoById failed.");

			CNodeInfo *pNode = (CNodeInfo *)pBuffer.get();
			
			SAFEBUFFER(pBufferNode, pNode->dwSize);
			ZeroMemory(pBufferNode.get(), pNode->dwSize);

			// seek to node offset
			m_lpFile->Seek(pNode->dwOffset, CFile::begin);

			if(m_lpFile->Read(pBufferNode.get(), pNode->dwSize) != pNode->dwSize)
				THROW("CollectNodesInfo: read failed.");

			// node name size
			ULONG ulNameSize = *(BYTE*)pBufferNode.get();

			// save node name
			CString csNodeName = (CString)(pBufferNode.get() + 1);
			csNodeName.SetAt(ulNameSize, '\0');

			// set base address
			ULONG ulBaseOffs = ulNameSize + 1 + 0x4C;
			
			vector<int> vHasChild;
			ULONG ulPaddedOffs = 0;

			for(int i=0; ; i++) {
				ULONG ulChkChild = *(ULONG*)(pBufferNode.get() + ulBaseOffs + (i * 4));
				if(ulChkChild <= m_lpHeader->dwNodeSize) {
					vHasChild.push_back(ulChkChild);
					ulPaddedOffs += 4;
				} else {
					break;
				}
			}

			// copy encrypted info to buffer, then decrypt it to get file count
			BYTE ucFileInfo[8];
			memcpy(&ucFileInfo, (void *)(pBufferNode.get() + ulBaseOffs + ulPaddedOffs), 8);
			BitRotateDecrypt(&ucFileInfo, 8, 3);

			// file count from decrypted info
			ULONG ulFileCount = *(ULONG*)(ucFileInfo + 4);

			CSingleNode Node(csNodeName, pNode->dwIndex, pNode->dwOffset, pNode->dwSize, ulFileCount);
			if(!vHasChild.empty())
				Node.ChildId = vHasChild;

			// file process
			bool bPadded = false;
			for(int i=0; i < (int)ulFileCount; i++) {
				ULONG ulRawOffs = ulBaseOffs + ulPaddedOffs + 8 + (i * (bPadded ? sizeof(CPackFileInfo) : sizeof(CPackFileInfo)-16));
				ULONG ulOffs = (ULONG)pBufferNode.get() + ulRawOffs;

				CPackFileInfo *pFileInfo = (CPackFileInfo *)ulOffs; // f7 3f95b7 // 3f9603

				if(pFileInfo->Ended == 0x01000000)
					bPadded = true;
				else
					bPadded = false;

				BitRotateDecrypt(pFileInfo, (bPadded ? sizeof(CPackFileInfo) : sizeof(CPackFileInfo)-16), 2);

				ULONG ulPushOffset	= bPadded ? (pFileInfo->OffsShift_2 << 0x10) | pFileInfo->OffsOr_2 : (pFileInfo->OffsShift_1 << 0x10) | pFileInfo->OffsOr_1;
				ULONG ulPushSize	= bPadded ? (pFileInfo->SizeShift_2 << 0x10) | pFileInfo->SizeOr_2 : (pFileInfo->SizeShift_1 << 0x10) | pFileInfo->SizeOr_1;

				Node.Files.push_back(CNodeFileInfo(pFileInfo->szFilename, 
												   ulPushOffset,
												   ulPushSize,
												   ulRawOffs,
												   bPadded
												  ));
			}

			Node.DirTableOffs = ulDirOffs;
			m_pvPackNodes->push_back(Node);

			m_pvHeaderDirInfo->push_back(CHeaderNodeInfo(ulDirOffs, pNode->dwIndex, pNode->dwOffset, pNode->dwSize));
		}
	}
	catch(char *e) 
	{
#ifdef _DEBUG
		AfxMessageBox(e);
#endif
		return FALSE;
	}

	return TRUE;
}

vector<CString> CI3Pack::SplitToVector( CString& str, char* pszSeps )
{
	vector<CString> result;
	char* psz = _strdup( str.GetBuffer(0));
	char* token = NULL;
	token  = strtok ( psz, pszSeps);
	while ( token) {
		CString sub = token;
		result.push_back ( sub);
		token = strtok( NULL, pszSeps);
	} 
	free (psz);
	return result;
}

BOOL CI3Pack::GetDirectoryInfoById( CNodeInfo **pNode, int nIndex, ULONG &ulOffset )
{
	try
	{
		if(!m_lpHeader)
			THROW("GetDirectoryInfoById: m_lpHeader == NULL");

		ULONG ulOff = m_lpHeader->dwDirTableOff + (nIndex * sizeof(CNodeInfo));
		m_lpFile->Seek(ulOff, CFile::begin);

		if(m_lpFile->Read(*pNode, sizeof(CNodeInfo)) != sizeof(CNodeInfo))
			THROW("GetDirectoryInfoById: read failed.");

		ulOffset = ulOff;
	}
	catch (char *e)
	{
#ifdef _DEBUG
		AfxMessageBox(e);
#endif
		return FALSE;
	}
	return TRUE;
}

int CI3Pack::PackRead( void **ppBuffer, UINT uiCount, UINT uiSeek/*=0*/ )
{
	try
	{
		if(!m_lpFile)
			THROW(1);

		m_lpFile->Seek(uiSeek, CFile::begin);

		UINT uiRet = m_lpFile->Read(*ppBuffer, uiCount);

		if(uiRet != uiCount)
			THROW(2);

	}
	catch(int e) {
		return e;
	}

	return 0;
}

ULONG CI3Pack::GetPackSize() {

	ULONG ulRet = 0;

	try
	{
		CFileStatus fs;
		if(!m_lpFile->GetStatus(fs))
			THROW(0);

		ulRet = (ULONG)fs.m_size;
	}
	catch(ULONG e)
	{
		return e;
	}
	return ulRet;
}

BOOL CI3Pack::IsFileOpened() {
	if(!m_lpFile)
		return FALSE;

	if(m_lpFile->m_hFile != INVALID_HANDLE_VALUE)
		return TRUE;

	return FALSE;
}

void CI3Pack::ClearBufferList() {
	if(!m_pvPackNodes->empty())
		m_pvPackNodes->clear();

	if(!m_pvHeaderDirInfo->empty())
		m_pvHeaderDirInfo->clear();
}

BOOL CI3Pack::ChangeFile( CString csNodeName, CString csFilename, void *pBuffer, UINT uiNewFileSize )
{
	void *pWriteBuffer = 0;
	try
	{
		if(csNodeName.IsEmpty() || csFilename.IsEmpty() || uiNewFileSize == 0)
			THROW("ChangeFile: Node name or file name is empty.");
		
		vector<CSingleNode>::iterator it;
		it = find_if(m_pvPackNodes->begin(), m_pvPackNodes->end(), boost::BOOST_BIND(&CSingleNode::NodeName, _1) == csNodeName);
		if(it == m_pvPackNodes->end())
			THROW("ChangeFile: node iterator not found.");

// 		vector<CHeaderNodeInfo>::iterator hit;
// 		hit = find_if(m_pvHeaderDirInfo->begin(), m_pvHeaderDirInfo->end(), boost::BOOST_BIND(&CHeaderNodeInfo::Index, _1) == it->Index);
// 		if(hit == m_pvHeaderDirInfo->end())
// 			THROW("ChangeFile: header node iterator not found.");

		vector<CNodeFileInfo>::iterator fit;
		fit = find_if(it->Files.begin(), it->Files.end(), boost::BOOST_BIND(&CNodeFileInfo::Filename, _1) == csFilename);
		if(fit == it->Files.end())
			THROW("ChangeFile: files iterator not found.");

		ULONG ulFirstOffs = fit->Offset;
		pWriteBuffer = malloc(ulFirstOffs);
		if(!pWriteBuffer)
			THROW("ChangeFile: could not allocate #1 buffer.");

		if(PackRead((void **)&pWriteBuffer, ulFirstOffs, 0) != 0)
			THROW("ChangeFile: could not read for #1 buffer.");

		void *pNewPtr = realloc(pWriteBuffer, ulFirstOffs+uiNewFileSize);
		if(!pNewPtr)
			THROW("ChangeFile: could not reallocate #2 buffer.");

		pWriteBuffer = pNewPtr;
		if(memcpy_s((void *)(ULONG(pWriteBuffer) + ulFirstOffs), uiNewFileSize, pBuffer, uiNewFileSize) != 0)
			THROW("ChangeFile: could not copy new file buffer to #2 buffer.");

		ULONG ulLastSectionSize = GetPackSize() - (fit->Offset + fit->Size);
	
		pNewPtr = realloc(pWriteBuffer, ulFirstOffs+uiNewFileSize+ulLastSectionSize);
		if(!pNewPtr)
			THROW("ChangeFile: could not reallocate #3 buffer.");

		pWriteBuffer = pNewPtr;

		void *pLastBuffer = (void *)(ULONG(pWriteBuffer) + ulFirstOffs + uiNewFileSize);
		if(PackRead((void **)&pLastBuffer, ulLastSectionSize, fit->Offset + fit->Size) != 0)
			THROW("ChangeFile: could not read for #3 buffer.");

		ULONG ulTotalSize = ulFirstOffs + uiNewFileSize + ulLastSectionSize;
		ULONG ulDiff = uiNewFileSize - fit->Size;

		for(vector<CSingleNode>::iterator _it = m_pvPackNodes->begin(); _it != m_pvPackNodes->end(); ++_it) {
			for(vector<CNodeFileInfo>::iterator _fit = _it->Files.begin(); _fit != _it->Files.end(); ++_fit) {
				if(_fit->Offset >= ulFirstOffs) {
					bool ChangeSize = _fit->Offset == ulFirstOffs;

					ULONG ulOffsNew = _fit->Offset + ulDiff;

					ULONG ulFilePackOffs = _it->Offset + _fit->RawOffset;

					ULONG ulFileInfoSz = _fit->Padded ? sizeof(CPackFileInfo) : sizeof(CPackFileInfo)-16;
					
					BYTE *pFIBuffer = new BYTE[ulFileInfoSz];
					memcpy_s(pFIBuffer, ulFileInfoSz, (void *)(ULONG(pWriteBuffer) + ulFilePackOffs), ulFileInfoSz);

					BitRotateDecrypt(pFIBuffer, ulFileInfoSz, 2);

					CPackFileInfo *pFileInfo = (CPackFileInfo *)pFIBuffer;

					if(_fit->Padded) {
						if(ChangeSize) {
							ULONG ulNewFsz = ((pFileInfo->SizeShift_2 << 0x10) | pFileInfo->SizeOr_2) + ulDiff;
							pFileInfo->SizeShift_2	= (WORD)(ulNewFsz >> 0x10);
							pFileInfo->SizeOr_2		= (WORD)((pFileInfo->SizeShift_2 << 0x10) ^ ulNewFsz);
						}

						if(!ChangeSize) {
							pFileInfo->OffsShift_2	= (WORD)(ulOffsNew >> 0x10);
							pFileInfo->OffsOr_2		= (WORD)((pFileInfo->OffsShift_2 << 0x10) ^ ulOffsNew);
						}
					} else {
						if(ChangeSize) {
							ULONG ulNewFsz = ((pFileInfo->SizeShift_1 << 0x10) | pFileInfo->SizeOr_1) + ulDiff;
							pFileInfo->SizeShift_1	= (WORD)(ulNewFsz >> 0x10);
							pFileInfo->SizeOr_1		= (WORD)((pFileInfo->SizeShift_1 << 0x10) ^ ulNewFsz);
						}

						if(!ChangeSize) {
							pFileInfo->OffsShift_1	= (WORD)(ulOffsNew >> 0x10);
							pFileInfo->OffsOr_1		= (WORD)((pFileInfo->OffsShift_1 << 0x10) ^ ulOffsNew);
						}
					}

					BitRotateEncrypt(pFIBuffer, ulFileInfoSz, 2);

					memcpy_s((void *)(ULONG(pWriteBuffer) + ulFilePackOffs), ulFileInfoSz, pFIBuffer, ulFileInfoSz);

					if(_fit->Offset == ulFirstOffs) {
						ULONG ulNewSize = uiNewFileSize;
					}

					delete pFIBuffer;
				}
			}

			CNodeInfo *pDir = (CNodeInfo *)(ULONG(pWriteBuffer) + _it->DirTableOffs);
			if(pDir->dwOffset > ulFirstOffs) {
				pDir->dwOffset += ulDiff;
				ASSERT(pDir != NULL);
			}
		}

		CString csFn = PathFindFileName(m_csFilePath);
		CFileDialog saveDlg(FALSE, "i3Pack", csFn, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "i3Pack File (*.i3Pack)|*.i3Pack||");
		
		if(saveDlg.DoModal() != IDOK)
			THROW(0);

		CString csFileSaveAs = saveDlg.GetPathName();
		
		CFile fileSave;
		if(fileSave.Open(csFileSaveAs, CFile::modeCreate | CFile::modeWrite) == FALSE)
			THROW("ChangeFile: could not create file.");
		
		fileSave.Seek(0, CFile::begin);
		fileSave.Write(pWriteBuffer, ulTotalSize);
		fileSave.Close();

		m_csReplacedFile = csFileSaveAs;

	}
	catch (char* e)
	{
		if(pWriteBuffer)
			free(pWriteBuffer);
#ifdef _DEBUG
		AfxMessageBox(e);
#endif // _DEBUG
		return FALSE;
	}

	if(pWriteBuffer)
		free(pWriteBuffer);	

	return TRUE;
}