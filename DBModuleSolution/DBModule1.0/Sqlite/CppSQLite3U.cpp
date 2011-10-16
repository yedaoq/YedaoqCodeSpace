////////////////////////////////////////////////////////////////////////////////
// CppSQLite3U is a C++ unicode wrapper around the SQLite3 embedded database library.
//
// Copyright (c) 2006  Tyushkov Nikolay.  All Rights Reserved. http://softvoile.com
//
//
// Based on beautiful wrapper written by Rob Groves  
// (https://secure.codeproject.com/database/CppSQLite.asp). 
// Very good wrapper, but without unicode support unfortunately. 
// So, I have reconstructed it for unicode.
//
//  CppSQLite3 wrapper:
//  Copyright (c) 2004 Rob Groves. All Rights Reserved. rob.groves@btinternet.com
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without a written
// agreement, is hereby granted, provided that the above copyright notice, 
// this paragraph and the following two paragraphs appear in all copies, 
// modifications, and distributions.
//
// IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
// PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
// ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". THE AUTHOR HAS NO OBLIGATION
// TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
// 
// If you want to get some documentation look at 
// https://secure.codeproject.com/database/CppSQLite.asp
// Note, not all features from CppSQLite3 were implemented in CppSQLite3U
//
// V1.0		11/06/2006	- Initial Public Version
//
//  Noteses : 
//   I have tested this wrapper only in unicode version, so I have no idea 
//   about its work in ANSI configuration, I think it doesn't work without modification;)
//
//  Home page : http://softvoile.com/development/CppSQLite3U/
//  Please send all bug report and comment to mail2@softvoile.com
// 
// 
////////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include "mytype.h"
#include "CppSQLite3U.h"
#include "boost\format.hpp"
#include "boost\lexical_cast.hpp"
#include "boost\algorithm\string.hpp"

#define MAX_SQL_SIZE 1024

/////////////////////////////////////////////////////////////////////////////
// CppSQLite3Exception

CppSQLite3Exception::CppSQLite3Exception(const int nErrCode,
									tchar* szErrMess,
									bool bDeleteMsg/*=true*/) :
									mnErrCode(nErrCode)
{
	int nLen = szErrMess ? _tcslen(szErrMess)+50 : 50;
	mpszErrMess=new tchar[ nLen ];

	tstring errMess = (tformat(TEXT("%s[%d]: %s"))
		%errorCodeAsString(nErrCode)
		%nErrCode
		%(szErrMess ? szErrMess : TEXT(""))).str();

	errMess.copy(mpszErrMess, errMess.length());

	if (bDeleteMsg && szErrMess)
	{
		_sqlite3_free((char*)szErrMess);
	}
}


CppSQLite3Exception::CppSQLite3Exception(const CppSQLite3Exception&  e) :
									mnErrCode(e.mnErrCode)
{
	mpszErrMess = 0;
	if (e.mpszErrMess)
	{
		int nLen = _tcslen(e.mpszErrMess)+10;
		mpszErrMess=new tchar[nLen];
		_tcscpy(mpszErrMess, e.mpszErrMess);
	}
}


tchar const * CppSQLite3Exception::errorCodeAsString(int nErrCode)
{
	switch (nErrCode)
	{
		case SQLITE_OK          : return TEXT("SQLITE_OK");
		case SQLITE_ERROR       : return TEXT("SQLITE_ERROR");
		case SQLITE_INTERNAL    : return TEXT("SQLITE_INTERNAL");
		case SQLITE_PERM        : return TEXT("SQLITE_PERM");
		case SQLITE_ABORT       : return TEXT("SQLITE_ABORT");
		case SQLITE_BUSY        : return TEXT("SQLITE_BUSY");
		case SQLITE_LOCKED      : return TEXT("SQLITE_LOCKED");
		case SQLITE_NOMEM       : return TEXT("SQLITE_NOMEM");
		case SQLITE_READONLY    : return TEXT("SQLITE_READONLY");
		case SQLITE_INTERRUPT   : return TEXT("SQLITE_INTERRUPT");
		case SQLITE_IOERR       : return TEXT("SQLITE_IOERR");
		case SQLITE_CORRUPT     : return TEXT("SQLITE_CORRUPT");
		case SQLITE_NOTFOUND    : return TEXT("SQLITE_NOTFOUND");
		case SQLITE_FULL        : return TEXT("SQLITE_FULL");
		case SQLITE_CANTOPEN    : return TEXT("SQLITE_CANTOPEN");
		case SQLITE_PROTOCOL    : return TEXT("SQLITE_PROTOCOL");
		case SQLITE_EMPTY       : return TEXT("SQLITE_EMPTY");
		case SQLITE_SCHEMA      : return TEXT("SQLITE_SCHEMA");
		case SQLITE_TOOBIG      : return TEXT("SQLITE_TOOBIG");
		case SQLITE_CONSTRAINT  : return TEXT("SQLITE_CONSTRAINT");
		case SQLITE_MISMATCH    : return TEXT("SQLITE_MISMATCH");
		case SQLITE_MISUSE      : return TEXT("SQLITE_MISUSE");
		case SQLITE_NOLFS       : return TEXT("SQLITE_NOLFS");
		case SQLITE_AUTH        : return TEXT("SQLITE_AUTH");
		case SQLITE_FORMAT      : return TEXT("SQLITE_FORMAT");
		case SQLITE_RANGE       : return TEXT("SQLITE_RANGE");
		case SQLITE_ROW         : return TEXT("SQLITE_ROW");
		case SQLITE_DONE        : return TEXT("SQLITE_DONE");
		case CPPSQLITE_ERROR    : return TEXT("CPPSQLITE_ERROR");
		default: return TEXT("UNKNOWN_ERROR");
	}
}


CppSQLite3Exception::~CppSQLite3Exception()
{
	if (mpszErrMess)
	{
 		delete [] mpszErrMess;
		mpszErrMess = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CppSQLite3DB

CppSQLite3DB::CppSQLite3DB()
{
	mpDB = 0;
	mnBusyTimeoutMs = 60000; // 60 seconds
}

CppSQLite3DB::CppSQLite3DB(const CppSQLite3DB& db)
{
	mpDB = db.mpDB;
	mnBusyTimeoutMs = 60000; // 60 seconds
}


CppSQLite3DB::~CppSQLite3DB()
{
	close();
}

void CppSQLite3DB::Destory()
{
	delete this;
}

////////////////////////////////////////////////////////////////////////////////

CppSQLite3DB& CppSQLite3DB::operator=(const CppSQLite3DB& db)
{
	mpDB = db.mpDB;
	mnBusyTimeoutMs = 60000; // 60 seconds
	return *this;
}

bool CppSQLite3DB::open(tchar const * szFile)
{
    int nRet;

#if defined(_UNICODE) || defined(UNICODE)

nRet = sqlite3_open16(szFile, &mpDB); // not tested under window 98 

#else // For Ansi Version
//*************-  Added by Begemot  szFile must be in unicode- 23/03/06 11:04 - ****
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx ((OSVERSIONINFO *) &osvi);
   
	if ( osvi.dwMajorVersion == 5) 
    {
          WCHAR pMultiByteStr[MAX_PATH+1];
          MultiByteToWideChar( CP_ACP, 0, szFile,
		                    _tcslen(szFile)+1, pMultiByteStr,   
			                sizeof(pMultiByteStr)/sizeof(pMultiByteStr[0]) );
          nRet = sqlite3_open16(pMultiByteStr, &mpDB);
    }
    else
          nRet = sqlite3_open(szFile,&mpDB);
#endif
//*************************
	if (nRet != SQLITE_OK)
	{
		tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
		return false;
	}
	setBusyTimeout(mnBusyTimeoutMs);

//	sqlite3_key(mpDB,_T("123456"),6);
//	sqlite3_rekey(mpDB,_T("123456"),6);
//	sqlite3_rekey(mpDB,0,0);

// 	if (nRet == SQLITE_OK && mpDB)
// 	{
// 		nRet = sqlite3_key(mpDB,_T("123456"),6);
// 	}
	if (SQLITE_OK == nRet)
		return true;
	else
		return false;
}

void CppSQLite3DB::close()
{
	if (mpDB)
	{
		try
		{//这里会抛出异常，先try catch一下 by lj at 2009.08

			int nRet = _sqlite3_close(mpDB);

			if (nRet != SQLITE_OK)
			{
#ifdef _DEBUG
				tchar const * szError = (tchar const *)_sqlite3_errmsg(mpDB);
				throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
#endif
			}

			mpDB = 0;
		}
		catch(...)
		{

		}

	}
}

I_CppSQLite3Statement* CppSQLite3DB::compileStatement2(tchar const * szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);

	return (I_CppSQLite3Statement*)new CppSQLite3Statement(mpDB, pVM);

}
CppSQLite3Statement CppSQLite3DB::compileStatement(tchar const * szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);

	return CppSQLite3Statement(mpDB, pVM);
}


bool CppSQLite3DB::tableExists(tchar const * szTable)
{
	//tchar szSQL[128];
	//_stprintf_s(szSQL, 128, TEXT("select count(*) from sqlite_master where type='table' and name='%s'"), szTable);

	tstring strSql = (tformat(TEXT("select count(*) from sqlite_master where type='table' and name='%s'"))%szTable).str();
	int nRet = execScalar(strSql.c_str());
	return (nRet > 0);
}

int CppSQLite3DB::execDMLV(tchar const * szFormat, ...)
{
	tchar buf[MAX_SQL_SIZE];

	va_list args;
	va_start(args, szFormat);

	vstprintf_s(buf, MAX_SQL_SIZE - 1, szFormat, args);
	//strSQL.FormatV(szFormat, args);	

	va_end(args);

 	int iResult = execDML(buf);
	return iResult;
}

int CppSQLite3DB::execDML(tchar const * szSQL)
{
	int nRet;
	sqlite3_stmt* pVM; 
	checkDB();

	do{ 
		pVM = compile(szSQL);

		nRet = _sqlite3_step(pVM);
	
		if (nRet == SQLITE_ERROR)
		{
			tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
		}
		nRet = _sqlite3_finalize(pVM);
	} 
	while( nRet == SQLITE_SCHEMA );
	return nRet;
}

CppSQLite3Query CppSQLite3DB::execQueryV(tchar const * szFormat, ...)
{
	tchar buf[MAX_SQL_SIZE];

	va_list args;
	va_start(args, szFormat);

	vstprintf_s(buf, MAX_SQL_SIZE - 1, szFormat, args);
	//strSQL.FormatV(szFormat, args);	

	va_end(args);

	return execQuery(buf);
}

I_CppSQLite3Query* CppSQLite3DB::execQueryV2(tchar const * szFormat, ...)
{
	tchar buf[MAX_SQL_SIZE];

	va_list args;
	va_start(args, szFormat);

	vstprintf_s(buf, MAX_SQL_SIZE - 1, szFormat, args);
	//strSQL.FormatV(szFormat, args);	

	va_end(args);

	return execQuery2(buf);
}
void CppSQLite3Query::Destory()
{
	delete this;
}


CppSQLite3Query CppSQLite3DB::execQuery(tchar const * szSQL)
{
	checkDB();
	int nRet;
	sqlite3_stmt* pVM; 
	
	do{ 
		pVM = compile(szSQL);

		nRet = _sqlite3_step(pVM);

		if (nRet == SQLITE_DONE)
		{	// no rows
			return CppSQLite3Query(mpDB, pVM, true/*eof*/);
		}
		else if (nRet == SQLITE_ROW)
		{	// at least 1 row
			return CppSQLite3Query(mpDB, pVM, false/*eof*/);
		}
		nRet = _sqlite3_finalize(pVM);
	} 
	while( nRet == SQLITE_SCHEMA ); // Edit By Begemot 08/16/06 12:44:35 -   read SQLite FAQ 
	
	tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
	throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
}

I_CppSQLite3Query* CppSQLite3DB::execQuery2(tchar const * szSQL)
{
	checkDB();
	int nRet;
	sqlite3_stmt* pVM; 
	
	do{ 
		pVM = compile(szSQL);

		nRet = _sqlite3_step(pVM);

		
		if (nRet == SQLITE_DONE)
		{	// no rows
 			I_CppSQLite3Query* pResult = new CppSQLite3Query(mpDB, pVM, true/*eof*/);
// 			pResult->AddRef();
			return pResult;
		}
		else if (nRet == SQLITE_ROW)
		{	// at least 1 row
			I_CppSQLite3Query* pResult = new CppSQLite3Query(mpDB, pVM, false/*eof*/);
//			pResult->AddRef();
			return pResult;
		}
		nRet = _sqlite3_finalize(pVM);
	} 
	while( nRet == SQLITE_SCHEMA ); // Edit By Begemot 08/16/06 12:44:35 -   read SQLite FAQ 
	
	tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
	throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
}
int CppSQLite3DB::execScalarV(tchar const * szFormat, ...)
{
	tchar buf[MAX_SQL_SIZE];

	va_list args;
	va_start(args, szFormat);

	vstprintf_s(buf, MAX_SQL_SIZE - 1, szFormat, args);
	//strSQL.FormatV(szFormat, args);	

	va_end(args);

	return execScalar(buf);
}

tstring CppSQLite3DB::execScalarStrV(tchar const * szFormat, ...)
{
	tchar buf[MAX_SQL_SIZE];

	va_list args;
	va_start(args, szFormat);

	vstprintf_s(buf, MAX_SQL_SIZE - 1, szFormat, args);
	//strSQL.FormatV(szFormat, args);	

	va_end(args);

	return tstring(execScalarStr(buf));
}

int CppSQLite3DB::execScalar(tchar const * szSQL)
{
	CppSQLite3Query q = execQuery(szSQL);

	if (q.eof() || q.numFields() < 1)
		throw CppSQLite3Exception(CPPSQLITE_ERROR, TEXT("Invalid scalar query"),	DONT_DELETE_MSG);

	return boost::lexical_cast<int>(q.fieldValue(0));
}

// Added By Begemot, exact as execScalar but return CString  08/06/06 16:30:37
tstring CppSQLite3DB::execScalarStr(tchar const * szSQL)
{
	CppSQLite3Query q = execQuery(szSQL);

	if (q.eof() || q.numFields() < 1)
		throw CppSQLite3Exception(CPPSQLITE_ERROR, TEXT("Invalid scalar query"),	DONT_DELETE_MSG);
	
	return tstring(q.getStringField(0));
}

sqlite_int64 CppSQLite3DB::lastRowId()
{
	return sqlite3_last_insert_rowid(mpDB);
}


void CppSQLite3DB::setBusyTimeout(int nMillisecs)
{
	mnBusyTimeoutMs = nMillisecs;
	sqlite3_busy_timeout(mpDB, mnBusyTimeoutMs);
}


void CppSQLite3DB::checkDB()
{
	if (!mpDB)
		throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Database not open"), DONT_DELETE_MSG);
	
}


sqlite3_stmt* CppSQLite3DB::compile(tchar const * szSQL)
{	
	checkDB();
	sqlite3_stmt* pVM;

	int nRet = _sqlite3_prepare(mpDB, szSQL, -1, &pVM, NULL);

	if (nRet != SQLITE_OK)
	{
		pVM=NULL;
		tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
	}
	return pVM;
}


//////////////////////// CppSQLite3Statement  ///////////////////////////////////////////
CppSQLite3Statement::CppSQLite3Statement()
{
	mpDB = 0;
	mpVM = 0;
}

CppSQLite3Statement::CppSQLite3Statement(const CppSQLite3Statement& rStatement)
{
	mpDB = rStatement.mpDB;
	mpVM = rStatement.mpVM;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
}

CppSQLite3Statement::CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM)
{
	mpDB = pDB;
	mpVM = pVM;
}

CppSQLite3Statement::~CppSQLite3Statement()
{
	try
	{
		finalize();
	}
	catch (...)	{}
}
void CppSQLite3Statement::Destory()
{
	delete this;
}

CppSQLite3Statement& CppSQLite3Statement::operator=(const CppSQLite3Statement& rStatement)
{
	mpDB = rStatement.mpDB;
	mpVM = rStatement.mpVM;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
	return *this;
}

int CppSQLite3Statement::execDML()
{
	checkDB();
	checkVM();

	int nRet = sqlite3_step(mpVM);

	if (nRet == SQLITE_DONE)
	{
		int nRowsChanged = sqlite3_changes(mpDB);

		nRet = sqlite3_reset(mpVM);

		if (nRet != SQLITE_OK)
		{
			tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
		}
		return nRowsChanged;
	}
	else
	{
		nRet = sqlite3_reset(mpVM);
		tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::bind(int nParam, tchar const * szValue)
{
	checkVM();
	int nRes = _sqlite3_bind_text(mpVM, nParam, szValue, -1, SQLITE_TRANSIENT);
	if (nRes != SQLITE_OK)
		throw CppSQLite3Exception(nRes,TEXT("Error binding string param"), DONT_DELETE_MSG);
}


void CppSQLite3Statement::bind(int nParam, const int nValue)
{
	checkVM();
	int nRes = sqlite3_bind_int(mpVM, nParam, nValue);
	if (nRes != SQLITE_OK)
		throw CppSQLite3Exception(nRes,TEXT("Error binding int param"), DONT_DELETE_MSG);
}


void CppSQLite3Statement::bind(int nParam, const double dValue)
{
	checkVM();
	int nRes = sqlite3_bind_double(mpVM, nParam, dValue);
	if (nRes != SQLITE_OK)
		throw CppSQLite3Exception(nRes,	TEXT("Error binding double param"), DONT_DELETE_MSG);
}


void CppSQLite3Statement::bind(int nParam, const unsigned char* blobValue, int nLen)
{
	checkVM();
	int nRes = sqlite3_bind_blob(mpVM, nParam,(const void*)blobValue, nLen, SQLITE_TRANSIENT);
	if (nRes != SQLITE_OK)
		throw CppSQLite3Exception(nRes,TEXT("Error binding blob param"),DONT_DELETE_MSG);
}


void CppSQLite3Statement::bindNull(int nParam)
{
	checkVM();
	int nRes = sqlite3_bind_null(mpVM, nParam);

	if (nRes != SQLITE_OK)
  		throw CppSQLite3Exception(nRes,TEXT("Error binding NULL param"),DONT_DELETE_MSG);
}


void CppSQLite3Statement::reset()
{
	if (mpVM)
	{
		int nRet = sqlite3_reset(mpVM);

		if (nRet != SQLITE_OK)
		{
			tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
		}
	}
}


void CppSQLite3Statement::finalize()
{
	if (mpVM)
	{
		int nRet = sqlite3_finalize(mpVM);
		mpVM = 0;

		if (nRet != SQLITE_OK)
		{
			tchar const * szError = (tchar const *) _sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
		}
	}
}


void CppSQLite3Statement::checkDB()
{
	if (mpDB == 0) throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Database not open"), DONT_DELETE_MSG);
}

void CppSQLite3Statement::checkVM()
{
	if (mpVM == 0)
		throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Null Virtual Machine pointer"), DONT_DELETE_MSG);
}


/////////////////////  CppSQLite3Query  //////////////////////////////////////////////////
CppSQLite3Query::CppSQLite3Query()
{
	mpVM = 0;
	mbEof = true;
	mnCols = 0;
	mbOwnVM = false;
}


CppSQLite3Query::CppSQLite3Query(const CppSQLite3Query& rQuery)
{
	mpVM = rQuery.mpVM;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
}


CppSQLite3Query::CppSQLite3Query(sqlite3* pDB, sqlite3_stmt* pVM,
								 bool bEof,	 bool bOwnVM/*=true*/)
{
	mpDB = pDB;
	mpVM = pVM;
	mbEof = bEof;
	mnCols = _sqlite3_column_count(mpVM);
	mbOwnVM = bOwnVM;
}

CppSQLite3Query::~CppSQLite3Query()
{
	try
	{
		finalize();
	}
	catch (...) {}
}


CppSQLite3Query& CppSQLite3Query::operator=(const CppSQLite3Query& rQuery)
{
	try
	{
		finalize();
	}
	catch (...)	{ }

	mpVM = rQuery.mpVM;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
	return *this;
}


int CppSQLite3Query::numFields()
{
	checkVM();
	return mnCols;
}


tchar const * CppSQLite3Query::fieldValue(int nField)
{
	checkVM();

	if (nField < 0 || nField > mnCols-1)
		throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Invalid field index requested"),DONT_DELETE_MSG);

	return (tchar const *)_sqlite3_column_text(mpVM, nField);
}


tchar const * CppSQLite3Query::fieldValue(tchar const * szField)
{
	int nField = fieldIndex(szField);
	return (tchar const *)_sqlite3_column_text(mpVM, nField);
}


int CppSQLite3Query::getIntField(int nField, int nNullValue/*=0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return _sqlite3_column_int(mpVM, nField);
	}
}


int CppSQLite3Query::getIntField(tchar const * szField, int nNullValue/*=0*/)
{
	int nField = fieldIndex(szField);
	return getIntField(nField, nNullValue);
}


double CppSQLite3Query::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return fNullValue;
	}
	else
	{
		return _sqlite3_column_double(mpVM, nField);
	}
}


double CppSQLite3Query::getFloatField(tchar const * szField, double fNullValue/*=0.0*/)
{
	int nField = fieldIndex(szField);
	return getFloatField(nField, fNullValue);
}


tchar const * CppSQLite3Query::getStringField(int nField, tchar const * szNullValue/*=""*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return szNullValue;
	}
	else
	{
		return (tchar const *)_sqlite3_column_text(mpVM, nField);
	}
}


tchar const * CppSQLite3Query::getStringField(tchar const * szField, tchar const * szNullValue/*=""*/)
{
	int nField = fieldIndex(szField);
	return getStringField(nField, szNullValue);
}


const unsigned char* CppSQLite3Query::getBlobField(int nField, int& nLen)
{
	checkVM();

	if (nField < 0 || nField > mnCols-1)
		throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Invalid field index requested"),DONT_DELETE_MSG);

	nLen = _sqlite3_column_bytes(mpVM, nField);
	return (const unsigned char*)sqlite3_column_blob(mpVM, nField);
}


const unsigned char* CppSQLite3Query::getBlobField(tchar const * szField, int& nLen)
{
	int nField = fieldIndex(szField);
	return getBlobField(nField, nLen);
}


bool CppSQLite3Query::fieldIsNull(int nField)
{
	return (fieldDataType(nField) == SQLITE_NULL);
}


bool CppSQLite3Query::fieldIsNull(tchar const * szField)
{
	int nField = fieldIndex(szField);
	return (fieldDataType(nField) == SQLITE_NULL);
}


int CppSQLite3Query::fieldIndex(tchar const * szField)
{
	checkVM();

	if (szField)
	{
		for (int nField = 0; nField < mnCols; nField++)
		{
			tchar const * szTemp = (tchar const *)_sqlite3_column_name(mpVM, nField);

			if (_tcscmp(szField, szTemp) == 0)
			{
				return nField;
			}
		}
	}
	throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Invalid field name requested"),DONT_DELETE_MSG);
}


tchar const * CppSQLite3Query::fieldName(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Invalid field index requested"),DONT_DELETE_MSG);
	}
	return (tchar const *)_sqlite3_column_name(mpVM, nCol);
}


tchar const * CppSQLite3Query::fieldDeclType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Invalid field index requested"),DONT_DELETE_MSG);
	}
	return (tchar const *)_sqlite3_column_decltype(mpVM, nCol);
}


int CppSQLite3Query::fieldDataType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Invalid field index requested"), DONT_DELETE_MSG);
	}
	return _sqlite3_column_type(mpVM, nCol);
}


bool CppSQLite3Query::eof()
{
	checkVM();
	return mbEof;
}


void CppSQLite3Query::nextRow()
{
	checkVM();

	int nRet = _sqlite3_step(mpVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		mbEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		nRet = _sqlite3_finalize(mpVM);
		mpVM = 0;
		tchar const * szError = (tchar const *)_sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet,	(tchar*)szError, DONT_DELETE_MSG);
	}
}


void CppSQLite3Query::finalize()
{
	if (mpVM && mbOwnVM)
	{
		int nRet = _sqlite3_finalize(mpVM);
		mpVM = 0;
		if (nRet != SQLITE_OK)
		{
			tchar const * szError = (tchar const *)_sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, (tchar*)szError, DONT_DELETE_MSG);
		}
	}
}

void CppSQLite3Query::checkVM()
{
	if (mpVM == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,TEXT("Null Virtual Machine pointer"),DONT_DELETE_MSG);
	}
}


////////////////////////////////////////////////////////////////////////////////
//**************************
//*************-  Added By Begemot - 28/02/06 20:25 - ****
tstring DoubleQuotes(tstring in)
{
	boost::replace_all(in, TEXT("\'"),TEXT("\'\'"));
	//in.Replace(TEXT("\'"),TEXT("\'\'"));
	return in;
}


bool CppSQLite3DB::columnExists(tchar const * szTable, tchar const * szColumn)
{
	tstring strColumn(szColumn);
	tstring strSQL = (tformat(TEXT("select * from %s")) % szTable).str();
	//strSQL.Format(TEXT("select * from %s"), szTable);
	CppSQLite3Query q = execQuery(strSQL.c_str());
	if(q.eof())
		return false;

	int nCount = q.numFields();
	for(int i=0; i<nCount; ++i)
	{
		if(strColumn==q.fieldName(i))
			return true;
	}

	return false;
}