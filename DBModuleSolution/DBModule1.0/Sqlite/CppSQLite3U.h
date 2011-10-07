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

#if !defined(AFX_CPPSQLITE3U_H__1B1BE273_2D1E_439C_946F_3CBD1C0EFD2F__INCLUDED_)
#define AFX_CPPSQLITE3U_H__1B1BE273_2D1E_439C_946F_3CBD1C0EFD2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CppSQLite3U.h : header file
//
#include "I_DBControl.h"
#include "sqlite3.h"
#include "mytype.h"
/////////////////////////////////////////////////////////////////////////////

#ifndef interface
#define interface struct
#endif

#define SQL_MAXSIZE    2048

#define CPPSQLITE_ERROR 1000
static const bool DONT_DELETE_MSG=false;


tstring DoubleQuotes(tstring in);

class CppSQLite3Query;
class CppSQLite3Statement;

class  _declspec(dllexport) CppSQLite3Exception
{
public:

    CppSQLite3Exception(const int nErrCode,
					    tchar*  szErrMess,
					    bool bDeleteMsg=true);

    CppSQLite3Exception(const CppSQLite3Exception&  e);

    virtual ~CppSQLite3Exception();

    const int errorCode() { return mnErrCode; }

    tchar const * errorMessage() { return mpszErrMess; }

    static tchar const *  errorCodeAsString(int nErrCode);

private:

    int mnErrCode;
    tchar*  mpszErrMess;
};


class CppSQLite3DB :public I_CppSQLite3DB
{
// Construction
public:
	CppSQLite3DB();

// Operations
public:

    virtual ~CppSQLite3DB();

    bool open(tchar const * szFile);

    void close();
    bool tableExists(tchar const * szTable);
	bool columnExists(tchar const * szTable, tchar const * szColumn);
    int execDML(tchar const * szSQL);
	int execDMLV(tchar const * szFormat, ...);

    CppSQLite3Query execQuery(tchar const * szSQL);
	CppSQLite3Query execQueryV(tchar const * szFormat, ...);
	I_CppSQLite3Query* execQuery2(tchar const * szSQL);//这个函数返回的CppSQLite3Query指针对象必须外部析构掉
	I_CppSQLite3Query* execQueryV2(tchar const * szFormat, ...);//这个函数返回的CppSQLite3Query指针对象必须外部析构掉


    int execScalar(tchar const * szSQL);
	tstring execScalarStr(tchar const * szSQL);
	int execScalarV(tchar const * szFormat, ...);
	tstring execScalarStrV(tchar const * szFormat, ...);

    CppSQLite3Statement compileStatement(tchar const * szSQL);
	I_CppSQLite3Statement* compileStatement2(tchar const * szSQL);

    sqlite_int64 lastRowId();

    void interrupt() { sqlite3_interrupt(mpDB); }

    void setBusyTimeout(int nMillisecs);

    static const char* SQLiteVersion() { return SQLITE_VERSION; }

	void Destory();

private:

    CppSQLite3DB(const CppSQLite3DB& db);
    CppSQLite3DB& operator=(const CppSQLite3DB& db);

    sqlite3_stmt* compile(tchar const * szSQL);

    void checkDB();
public:
    sqlite3* mpDB;
    int mnBusyTimeoutMs;
};
/////////////////////////////////////////////////////////////////////////////

class CppSQLite3Statement : public I_CppSQLite3Statement
{
public:

	CppSQLite3Statement();

	CppSQLite3Statement(const CppSQLite3Statement& rStatement);

	CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM);

	virtual ~CppSQLite3Statement();

	CppSQLite3Statement& operator=(const CppSQLite3Statement& rStatement);

	int execDML();

	template<typename T1>
	int execDMLV(T1 t1)
	{
		int i=1;
		bind(i++, t1);
		int nRet = execDML();
        reset();
		return nRet;
	}

	template<typename T1, typename T2>
	int execDMLV(T1 t1, T2 t2)
	{
		int i=1;
		bind(i++, t1);
		bind(i++, t2);
		int nRet = execDML();
        reset();
		return nRet;
	}

	template<typename T1, typename T2, typename T3>
	int execDMLV(T1 t1, T2 t2, T3 t3)
	{
		int i=1;
		bind(i++, t1);
		bind(i++, t2);
		bind(i++, t3);
		int nRet = execDML();
        reset();
		return nRet;
	}

	CppSQLite3Query execQuery();

	void bind(int nParam, tchar const * szValue);
	void bind(int nParam, const int nValue);
	void bind(int nParam, const double dwValue);
	void bind(int nParam, const unsigned char* blobValue, int nLen);
	void bindNull(int nParam);

	void reset();

	void finalize();
	void Destory();
private:

	void checkDB();
	void checkVM();

	sqlite3* mpDB;
	sqlite3_stmt* mpVM;
};
/////////////////////  CppSQLite3Query  //////////////////////////////////////////////////
class CppSQLite3Query:public I_CppSQLite3Query
{
public:

	CppSQLite3Query();

	CppSQLite3Query(const CppSQLite3Query& rQuery);

	CppSQLite3Query(sqlite3* pDB,
		sqlite3_stmt* pVM,
		bool bEof,
		bool bOwnVM=true);

	CppSQLite3Query& operator=(const CppSQLite3Query& rQuery);

	virtual ~CppSQLite3Query();

	int numFields();

	int fieldIndex(tchar const * szField);
	tchar const * fieldName(int nCol);

	tchar const * fieldDeclType(int nCol);
	int fieldDataType(int nCol);

	tchar const * fieldValue(int nField);
	tchar const * fieldValue(tchar const * szField);

	int getIntField(int nField, int nNullValue=0);
	int getIntField(tchar const * szField, int nNullValue=0);

	double getFloatField(int nField, double fNullValue=0.0);
	double getFloatField(tchar const * szField, double fNullValue=0.0);

	tchar const * getStringField(int nField, tchar const * szNullValue=TEXT(""));
	tchar const * getStringField(tchar const * szField, tchar const * szNullValue=TEXT(""));

	const unsigned char* getBlobField(int nField, int& nLen);
	const unsigned char* getBlobField(tchar const * szField, int& nLen);

	bool fieldIsNull(int nField);
	bool fieldIsNull(tchar const * szField);

	bool eof();
	void nextRow();
	void finalize();
	void Destory();

private:

	void checkVM();

	sqlite3* mpDB;
	sqlite3_stmt* mpVM;
	bool mbEof;
	int mnCols;
	bool mbOwnVM;
};

/////////////////////////////////////////////////////////////////////////////
//
// TCHAR based sqlite3 function names for Unicode/MCBS builds.
//
#if defined(_UNICODE) || defined(UNICODE)
//#pragma message("Unicode Selected")
#define _sqlite3_aggregate_context      sqlite3_aggregate_context
#define _sqlite3_aggregate_count        sqlite3_aggregate_count
#define _sqlite3_bind_blob              sqlite3_bind_blob
#define _sqlite3_bind_double            sqlite3_bind_double
#define _sqlite3_bind_int               sqlite3_bind_int
#define _sqlite3_bind_int64             sqlite3_bind_int64
#define _sqlite3_bind_null              sqlite3_bind_null
#define _sqlite3_bind_parameter_count   sqlite3_bind_parameter_count
#define _sqlite3_bind_parameter_index   sqlite3_bind_parameter_index
#define _sqlite3_bind_parameter_name    sqlite3_bind_parameter_name
#define _sqlite3_bind_text              sqlite3_bind_text16
#define _sqlite3_bind_text16            sqlite3_bind_text16
#define _sqlite3_busy_handler           sqlite3_busy_handler
#define _sqlite3_busy_timeout           sqlite3_busy_timeout
#define _sqlite3_changes                sqlite3_changes
#define _sqlite3_close                  sqlite3_close
#define _sqlite3_collation_needed       sqlite3_collation_needed16
#define _sqlite3_collation_needed16     sqlite3_collation_needed16
#define _sqlite3_column_blob            sqlite3_column_blob
#define _sqlite3_column_bytes           sqlite3_column_bytes16
#define _sqlite3_column_bytes16         sqlite3_column_bytes16
#define _sqlite3_column_count           sqlite3_column_count
#define _sqlite3_column_decltype        sqlite3_column_decltype16
#define _sqlite3_column_decltype16      sqlite3_column_decltype16
#define _sqlite3_column_double          sqlite3_column_double
#define _sqlite3_column_int             sqlite3_column_int
#define _sqlite3_column_int64           sqlite3_column_int64
#define _sqlite3_column_name            sqlite3_column_name16
#define _sqlite3_column_name16          sqlite3_column_name16
#define _sqlite3_column_text            sqlite3_column_text16
#define _sqlite3_column_text16          sqlite3_column_text16
#define _sqlite3_column_type            sqlite3_column_type
#define _sqlite3_commit_hook            sqlite3_commit_hook
#define _sqlite3_complete               sqlite3_complete16
#define _sqlite3_complete16             sqlite3_complete16
#define _sqlite3_create_collation       sqlite3_create_collation16
#define _sqlite3_create_collation16     sqlite3_create_collation16
#define _sqlite3_create_function        sqlite3_create_function16
#define _sqlite3_create_function16      sqlite3_create_function16
#define _sqlite3_data_count             sqlite3_data_count
#define _sqlite3_errcode                sqlite3_errcode
#define _sqlite3_errmsg                 sqlite3_errmsg16
#define _sqlite3_errmsg16               sqlite3_errmsg16
#define _sqlite3_exec                   sqlite3_exec
#define _sqlite3_finalize               sqlite3_finalize
#define _sqlite3_free                   sqlite3_free
#define _sqlite3_free_table             sqlite3_free_table
#define _sqlite3_get_table              sqlite3_get_table
#define _sqlite3_interrupt              sqlite3_interrupt
#define _sqlite3_last_insert_rowid      sqlite3_last_insert_rowid
#define _sqlite3_libversion             sqlite3_libversion
#define _sqlite3_mprintf                sqlite3_mprintf
#define _sqlite3_open                   sqlite3_open16
#define _sqlite3_open16                 sqlite3_open16
#define _sqlite3_prepare                sqlite3_prepare16
#define _sqlite3_prepare16              sqlite3_prepare16
#define _sqlite3_progress_handler       sqlite3_progress_handler
#define _sqlite3_reset                  sqlite3_reset
#define _sqlite3_result_blob            sqlite3_result_blob
#define _sqlite3_result_double          sqlite3_result_double
#define _sqlite3_result_error           sqlite3_result_error16
#define _sqlite3_result_error16         sqlite3_result_error16
#define _sqlite3_result_int             sqlite3_result_int
#define _sqlite3_result_int64           sqlite3_result_int64
#define _sqlite3_result_null            sqlite3_result_null
#define _sqlite3_result_text            sqlite3_result_text16
#define _sqlite3_result_text16          sqlite3_result_text16
#define _sqlite3_result_text16be        sqlite3_result_text16be
#define _sqlite3_result_text16le        sqlite3_result_text16le
#define _sqlite3_result_value           sqlite3_result_value
#define _sqlite3_set_authorizer         sqlite3_set_authorizer
#define _sqlite3_step                   sqlite3_step
#define _sqlite3_total_changes          sqlite3_total_changes
#define _sqlite3_trace                  sqlite3_trace
#define _sqlite3_user_data              sqlite3_user_data
#define _sqlite3_value_blob             sqlite3_value_blob
#define _sqlite3_value_bytes            sqlite3_value_bytes16
#define _sqlite3_value_bytes16          sqlite3_value_bytes16
#define _sqlite3_value_double           sqlite3_value_double
#define _sqlite3_value_int              sqlite3_value_int
#define _sqlite3_value_int64            sqlite3_value_int64
#define _sqlite3_value_text             sqlite3_value_text16
#define _sqlite3_value_text16           sqlite3_value_text16
#define _sqlite3_value_text16be         sqlite3_value_text16be
#define _sqlite3_value_text16le         sqlite3_value_text16le
#define _sqlite3_value_type             sqlite3_value_type
#define _sqlite3_vmprintf               sqlite3_vmprintf
#else
#pragma message("MCBS Selected")
#define _sqlite3_aggregate_context      sqlite3_aggregate_context
#define _sqlite3_aggregate_count        sqlite3_aggregate_count
#define _sqlite3_bind_blob              sqlite3_bind_blob
#define _sqlite3_bind_double            sqlite3_bind_double
#define _sqlite3_bind_int               sqlite3_bind_int
#define _sqlite3_bind_int64             sqlite3_bind_int64
#define _sqlite3_bind_null              sqlite3_bind_null
#define _sqlite3_bind_parameter_count   sqlite3_bind_parameter_count
#define _sqlite3_bind_parameter_index   sqlite3_bind_parameter_index
#define _sqlite3_bind_parameter_name    sqlite3_bind_parameter_name
#define _sqlite3_bind_text              sqlite3_bind_text
#define _sqlite3_bind_text16            sqlite3_bind_text16
#define _sqlite3_busy_handler           sqlite3_busy_handler
#define _sqlite3_busy_timeout           sqlite3_busy_timeout
#define _sqlite3_changes                sqlite3_changes
#define _sqlite3_close                  sqlite3_close
#define _sqlite3_collation_needed       sqlite3_collation_needed
#define _sqlite3_collation_needed16     sqlite3_collation_needed16
#define _sqlite3_column_blob            sqlite3_column_blob
#define _sqlite3_column_bytes           sqlite3_column_bytes
#define _sqlite3_column_bytes16         sqlite3_column_bytes16
#define _sqlite3_column_count           sqlite3_column_count
#define _sqlite3_column_decltype        sqlite3_column_decltype
#define _sqlite3_column_decltype16      sqlite3_column_decltype16
#define _sqlite3_column_double          sqlite3_column_double
#define _sqlite3_column_int             sqlite3_column_int
#define _sqlite3_column_int64           sqlite3_column_int64
#define _sqlite3_column_name            sqlite3_column_name
#define _sqlite3_column_name16          sqlite3_column_name16
#define _sqlite3_column_text            sqlite3_column_text
#define _sqlite3_column_text16          sqlite3_column_text16
#define _sqlite3_column_type            sqlite3_column_type
#define _sqlite3_commit_hook            sqlite3_commit_hook
#define _sqlite3_complete               sqlite3_complete
#define _sqlite3_complete16             sqlite3_complete16
#define _sqlite3_create_collation       sqlite3_create_collation
#define _sqlite3_create_collation16     sqlite3_create_collation16
#define _sqlite3_create_function        sqlite3_create_function
#define _sqlite3_create_function16      sqlite3_create_function16
#define _sqlite3_data_count             sqlite3_data_count
#define _sqlite3_errcode                sqlite3_errcode
#define _sqlite3_errmsg                 sqlite3_errmsg
#define _sqlite3_errmsg16               sqlite3_errmsg16
#define _sqlite3_exec                   sqlite3_exec
#define _sqlite3_finalize               sqlite3_finalize
#define _sqlite3_free                   sqlite3_free
#define _sqlite3_free_table             sqlite3_free_table
#define _sqlite3_get_table              sqlite3_get_table
#define _sqlite3_interrupt              sqlite3_interrupt
#define _sqlite3_last_insert_rowid      sqlite3_last_insert_rowid
#define _sqlite3_libversion             sqlite3_libversion
#define _sqlite3_mprintf                sqlite3_mprintf
#define _sqlite3_open                   sqlite3_open
#define _sqlite3_open16                 sqlite3_open16
#define _sqlite3_prepare                sqlite3_prepare
#define _sqlite3_prepare16              sqlite3_prepare16
#define _sqlite3_progress_handler       sqlite3_progress_handler
#define _sqlite3_reset                  sqlite3_reset
#define _sqlite3_result_blob            sqlite3_result_blob
#define _sqlite3_result_double          sqlite3_result_double
#define _sqlite3_result_error           sqlite3_result_error
#define _sqlite3_result_error16         sqlite3_result_error16
#define _sqlite3_result_int             sqlite3_result_int
#define _sqlite3_result_int64           sqlite3_result_int64
#define _sqlite3_result_null            sqlite3_result_null
#define _sqlite3_result_text            sqlite3_result_text
#define _sqlite3_result_text16          sqlite3_result_text16
#define _sqlite3_result_text16be        sqlite3_result_text16be
#define _sqlite3_result_text16le        sqlite3_result_text16le
#define _sqlite3_result_value           sqlite3_result_value
#define _sqlite3_set_authorizer         sqlite3_set_authorizer
#define _sqlite3_step                   sqlite3_step
#define _sqlite3_total_changes          sqlite3_total_changes
#define _sqlite3_trace                  sqlite3_trace
#define _sqlite3_user_data              sqlite3_user_data
#define _sqlite3_value_blob             sqlite3_value_blob
#define _sqlite3_value_bytes            sqlite3_value_bytes
#define _sqlite3_value_bytes16          sqlite3_value_bytes16
#define _sqlite3_value_double           sqlite3_value_double
#define _sqlite3_value_int              sqlite3_value_int
#define _sqlite3_value_int64            sqlite3_value_int64
#define _sqlite3_value_text             sqlite3_value_text
#define _sqlite3_value_text16           sqlite3_value_text16
#define _sqlite3_value_text16be         sqlite3_value_text16be
#define _sqlite3_value_text16le         sqlite3_value_text16le
#define _sqlite3_value_type             sqlite3_value_type
#define _sqlite3_vmprintf               sqlite3_vmprintf
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPPSQLITE3U_H__1B1BE273_2D1E_439C_946F_3CBD1C0EFD2F__INCLUDED_)
