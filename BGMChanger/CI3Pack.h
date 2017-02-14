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


#ifndef CI3Pack_h__
#define CI3Pack_h__

#include <algorithm>
#include <vector>

using namespace std;

#include <boost/shared_ptr.hpp>
using namespace boost;

#include "../i3PackTool/Structures.h"

#define ALLOCBUFFER(sz)		shared_ptr<BYTE>((BYTE *)malloc(sz), free)
#define ZEROBUFFER(obj, sz)	ZeroMemory(obj.get(), sz)
#define SAFEBUFFER(v, sz)	boost::shared_ptr<BYTE> v((BYTE *)malloc(sz), free)

class CI3Pack {
public:
	enum TableType {
		TTString,
		TTDirectory
	};

	enum TableInfo {
		TIOffset,
		TISize
	};

public:
	CI3Pack();
	~CI3Pack();

	int Open(CString csFile);

	BOOL Close();
	
	BOOL HasFileOpened() const { 
		return (BOOL)m_lpFile; 
	}
	
	BOOL GetHeaderInfo(CPackHeader **pHeader);

	CString GetFileID();

	int GetNodeSize();

	ULONG GetTableInfo(enum TableType type, enum TableInfo info);

	BOOL CollectNodesInfo();

	BOOL GetNodeStringInfo(CString &str);

	vector<CString> SplitToVector(CString& str, char* pszSeps);

	BOOL GetDirectoryInfoById(CNodeInfo **pNode, int nIndex, ULONG &ulOffset);

	vector<CSingleNode> *GetNodes() {
		return m_pvPackNodes;
	}

	vector<CHeaderNodeInfo> *GetDirs() {
		return m_pvHeaderDirInfo;
	}


	int PackRead(void **ppBuffer, UINT uiCount, UINT uiSeek=0);

	BOOL IsFileOpened();

	void ClearBufferList();

	CI3Pack *GetThis() {
		return this;
	}

	CFile *GetFileObject() {
		return m_lpFile;
	}

	ULONG GetPackSize();

	BOOL ChangeFile(CString csNodeName, CString csFilename, void *pBuffer, UINT uiSize);

public:
	CString m_csFilePath;
	CFile *m_lpFile;
	CFileException *m_lpErr;
	BOOL m_bIsFileOpen;

	CString m_csReplacedFile;
	
	CPackHeader *m_lpHeader;
	
	CString m_csNodeInfo;

	vector<CSingleNode> *m_pvPackNodes;
	vector<CHeaderNodeInfo> *m_pvHeaderDirInfo;

	
protected:
	CString GetErrorMessage(CFileException *e);
	CString GetErrorMessage(CException *e);

private:
	// v((BYTE *)malloc(sz), free)
	shared_ptr<BYTE> m_HeaderBuffer;

	vector<CSingleNode> *pvNodes;
};

#endif // CI3Pack_h__
