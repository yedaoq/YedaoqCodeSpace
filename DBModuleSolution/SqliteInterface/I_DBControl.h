#pragma once
//#include "..\..\include\IRefObject.h"
#include "mytype.h"

#ifndef interface
#define interface struct
#endif

interface I_CppSQLite3Query //: public IRefObject
{
public:
	virtual int numFields() = 0;

	virtual int fieldIndex(tchar const * szField) = 0;
	virtual tchar const * fieldName(int nCol) = 0;

	virtual tchar const * fieldDeclType(int nCol) = 0;
	virtual int fieldDataType(int nCol) = 0;

	virtual tchar const * fieldValue(int nField) = 0;
	virtual tchar const * fieldValue(tchar const * szField) = 0;

	virtual int getIntField(int nField, int nNullValue=0) = 0;
	virtual int getIntField(tchar const * szField, int nNullValue=0) = 0;

	virtual double getFloatField(int nField, double fNullValue=0.0) = 0;
	virtual double getFloatField(tchar const * szField, double fNullValue=0.0) = 0;

	virtual tchar const * getStringField(int nField, tchar const * szNullValue=TEXT("")) = 0;
	virtual tchar const * getStringField(tchar const * szField, tchar const * szNullValue=TEXT("")) = 0;

	virtual const unsigned char* getBlobField(int nField, int& nLen) = 0;
	virtual const unsigned char* getBlobField(tchar const * szField, int& nLen) = 0;

	virtual bool fieldIsNull(int nField) = 0;
	virtual bool fieldIsNull(tchar const * szField) = 0;


	virtual bool eof() = 0;
	virtual void nextRow() = 0;
	virtual void finalize() = 0;

	virtual void Destory() = 0;
};
interface I_CppSQLite3Statement
{
public:
	virtual int execDML() = 0;

	virtual void bind(int nParam, tchar const * szValue) = 0;
	virtual void bind(int nParam, const int nValue) = 0;
	virtual void bind(int nParam, const double dwValue) = 0;
	virtual void bind(int nParam, const unsigned char* blobValue, int nLen) = 0;
	virtual void bindNull(int nParam) = 0;

	virtual void reset() = 0;

	virtual void finalize() = 0;
	virtual void Destory() = 0;

};
interface I_CppSQLite3DB //: public IRefObject
{
public:
	virtual bool open(tchar const * szFile) = 0;

	virtual void close() = 0;
	virtual bool tableExists(tchar const * szTable) = 0;
	virtual bool columnExists(tchar const * szTable, tchar const * szColumn) = 0;
	virtual int execDML(tchar const * szSQL) = 0;
	virtual int execDMLV(tchar const * szFormat, ...) = 0;

	virtual I_CppSQLite3Query* execQuery2(tchar const * szSQL) = 0;
	virtual I_CppSQLite3Query* execQueryV2(tchar const * szFormat, ...) = 0;

	virtual int execScalar(tchar const * szSQL) = 0;
	virtual tstring execScalarStr(tchar const * szSQL) = 0;
	virtual int execScalarV(tchar const * szFormat, ...) = 0;
	virtual tstring execScalarStrV(tchar const * szFormat, ...) = 0;
	virtual void Destory() = 0;

	virtual I_CppSQLite3Statement* compileStatement2(tchar const * szSQL) = 0;

};

template <class T>
class CAutoDBObjPtr
{
	T* p;
public:
	CAutoDBObjPtr(){ p = NULL;}
	CAutoDBObjPtr(T* lp) { p = lp;}

	~CAutoDBObjPtr()
	{
		if (p)
		{
			p->Destory();
			p = 0;
		}
	}

	operator T*()
	{
		return p;
	}

	T* operator=(T* lp)
	{
		if(p != lp)
		{
			T* temp = p;
			p = lp;
			if (temp) temp->Destory();
		}
		
		return p;
	}

	T* operator->()
	{
		return p;
	}

	bool operator!()
	{
		return (p == NULL);
	}
	bool operator<(T* pT)
	{
		return p < pT;
	}
	bool operator!=(T* pT) const
	{
		return !operator==(pT);
	}
	bool operator==(T* pT)
	{
		return p == pT;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Destory();
		}
	}

	// Detach the interface (does not Release)
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}

};