#include "StdAfx.h"
#include "DwarfViewInfoDBTblFileInfo.h"
#include <Helper.h>
#include "DwarfOptions.h"
#include "mytype.h"
#include "fstream"
#include "iosfwd"

#pragma comment(lib, "Version.lib")

int CDwarfViewInfoDBTable<TBL_FileInfo>::InitializeOperations()
{
	__super::InitializeOperations();

	this->Operations.Append(TEXT("信息更新"), static_cast<DelegateOperation>(&CDwarfViewInfoDBTable<TBL_FileInfo>::OnFileinfoUpdate));

	return 1;
}

void CDwarfViewInfoDBTable<TBL_FileInfo>::OnFileinfoUpdate(DwarfViewOperationContext* pCtx)
{
	CDwarfView* pMainView = CGlobalData::GetViewByID(pCtx->MainViewID);
	if(!pMainView) return;

	IDBRecord* pRecFocused = pMainView->GetFocusedRecord();
	if(pRecFocused)
	{
		UpdateFileInfo(*pRecFocused);
		return;
	}

	std::auto_ptr<IEnumerator<IDBRecord>> pEnumRec(pMainView->GetSelectedRecords());
	if(!pEnumRec.get())
	{
		ASSERT(false);
		return;
	}

	CDBRecordAuto rec;
	while(pEnumRec->MoveNext())
	{
		UpdateFileInfo(pEnumRec->Current());
	}
}

//////////////////////////////////////////////////////////////////////////
// GetFileVersion : 获取文件版本字符串
BOOL GetFileVersion( LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen )
{
	TCHAR  szVersionBuffer[8192] = TEXT("");
	DWORD dwVerSize;
	DWORD dwHandle;

	dwVerSize = GetFileVersionInfoSize( strFile, &dwHandle );
	if( dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer)-1) )
		return FALSE;

	if( GetFileVersionInfo( strFile,0,dwVerSize,szVersionBuffer) )
	{
		VS_FIXEDFILEINFO * pInfo;
		unsigned int nInfoLen;

		if( VerQueryValue( szVersionBuffer, TEXT("\\"), (void**)&pInfo, &nInfoLen ) )
		{
			memset( pszVersion, 0, nVersionLen * sizeof(TCHAR));
			_stprintf_s( pszVersion, nVersionLen-1, TEXT("%d.%d.%d.%d"), 
				HIWORD( pInfo->dwFileVersionMS ), LOWORD( pInfo->dwFileVersionMS ), 
				HIWORD( pInfo->dwFileVersionLS ), LOWORD( pInfo->dwFileVersionLS ) );
			return TRUE;
		}
	}

	return FALSE;
}

__int64 GetFileSize(LPCTSTR lpFileName)
{
	WIN32_FIND_DATA FileData={0}; 
	HANDLE hSearch;
	hSearch = FindFirstFile(lpFileName, &FileData); 
	LARGE_INTEGER lnRet;
	if (hSearch != INVALID_HANDLE_VALUE) 
	{
		lnRet.LowPart = FileData.nFileSizeLow;
		lnRet.HighPart = FileData.nFileSizeHigh;
	}

	FindClose(hSearch);
	return lnRet.QuadPart;	
}

typedef struct tagMD5_CTX
{
	ULONG count[2];					/* number of bits, modulo 2^64 (lsb first) */
	ULONG buf[4];                   /* state (ABCD) */
	unsigned char in[64];           /* input buffer */
	unsigned char digest[16];       /* actual digest after MD5Final call */
}MD5_CTX;
typedef void (WINAPI* PMD5Init)(MD5_CTX *);
typedef void (WINAPI* PMD5Update)(MD5_CTX *, const unsigned char *, unsigned int);
typedef void (WINAPI* PMD5Final )(MD5_CTX *);
static PMD5Init MD5Init = 0;
static PMD5Update MD5Update = 0;
static PMD5Final MD5Final = 0;
void InputBuffer(LPBYTE lpBuffer,ULONG len,LPTSTR lpOutBuf)
{

	MD5_CTX _MD5CTX;
	MD5Init(&_MD5CTX);
	ZeroMemory(lpOutBuf,sizeof(TCHAR) * 33);

	MD5Update(&_MD5CTX,lpBuffer,len);

	MD5Final(&_MD5CTX);

	for(int i=0;i<16;i++)
	{
		TCHAR t[3];
		_stprintf_s(t,3 , _T("%02X"), _MD5CTX.digest[i]);
		StrCat(lpOutBuf,t);
	}
}

BOOL InputFileHandle(HANDLE hFile,DWORD dwPos,DWORD dwSplen,LPTSTR lpOutStr)
{
	if(!MD5Init)
	{
		HMODULE hMod = GetModuleHandle(_T("cryptdll.dll"));
		if(!hMod)
			hMod = LoadLibrary(_T("cryptdll.dll"));
		MD5Init = (PMD5Init)GetProcAddress(hMod,"MD5Init");
		MD5Update = (PMD5Update)GetProcAddress(hMod,"MD5Update");
		MD5Final= (PMD5Final)GetProcAddress(hMod,"MD5Final");
		if(!MD5Final || !MD5Update || !MD5Init)
			return FALSE;
	}
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	LPBYTE lpBuffer = NULL; 
	LPSTR lpNewMD5 = NULL;
	DWORD dwLen,dwRead;
	dwLen = GetFileSize(hFile,NULL);

	if(dwPos>dwLen)
		return FALSE;
	if(dwSplen!=0&&dwSplen!=-1)
	{
		if((dwPos + dwSplen)>dwLen)
			return FALSE;
		dwLen = dwSplen;
	}

	if(dwPos!=0)
		SetFilePointer(hFile,dwPos,NULL,FILE_BEGIN);

	lpBuffer = new BYTE[dwLen];

	ReadFile(hFile,lpBuffer,dwLen,&dwRead,NULL);

	InputBuffer(lpBuffer,dwLen,lpOutStr);

	delete []lpBuffer;

	return TRUE;		

}

BOOL InputFile(LPCTSTR lpFile,DWORD dwPos,DWORD dwSplen,LPTSTR lpOutStr)
{
	HANDLE hFile = CreateFile(lpFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	BOOL bCheck = InputFileHandle(hFile, dwPos, dwSplen,lpOutStr);
	CloseHandle(hFile);
	return bCheck;
}

void CDwarfViewInfoDBTable<TBL_FileInfo>::UpdateFileInfo( const IDBRecord& recFile ) 
{
	TTRACE(TEXT("更新文件信息 : %s\n"), recFile.GetField(COL_FileInfo_FileName).c_str());

	CDBRecordAuto rec = recFile;

	TCHAR szFilePath[MAX_PATH + 1] = {0};
	CDwarfOptions::GetInstance().DirCodecFile.copy(szFilePath, MAX_PATH);
	PathAppend(szFilePath, rec.GetField(COL_FileInfo_FileName).c_str());

	if(_taccess(szFilePath, 0))
	{
		MsgboxPrompt(TEXT("未找到文件 : %s"), szFilePath);
		return;
	}

	TCHAR buf[1024];

	rec.SetField(COL_FileInfo_FileSize, ToStr(GetFileSize(szFilePath)));

	if(GetFileVersion(szFilePath, buf, 1024))
	{
		rec.SetField(COL_FileInfo_FileVer, buf);
	}

	InputFile(szFilePath, 0, 0, buf);
	rec.SetField(COL_FileInfo_FileMd5, buf);

	if(DBModule->Tables()[TBL_FileInfo]->Update(rec, recFile) > 0)
	{
		
	}
}