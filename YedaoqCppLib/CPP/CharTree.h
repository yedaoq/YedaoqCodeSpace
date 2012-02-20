// CharTree.h: interface for the CCharTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTREE_H__540C5EC8_789D_4BAC_8A88_B3BC23214E2E__INCLUDED_)
#define AFX_CHARTREE_H__540C5EC8_789D_4BAC_8A88_B3BC23214E2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "tstring.h"

class CCharTree  
{
public:
	CCharTree();
	virtual ~CCharTree();

	virtual int Insert(const tchar* str);
	virtual int Find(const tchar* str, void* data);
	virtual int Delete(const tchar* str);
};

#endif // !defined(AFX_CHARTREE_H__540C5EC8_789D_4BAC_8A88_B3BC23214E2E__INCLUDED_)
