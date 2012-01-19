/********************************************************************
	created:	2011-6-16 16:41
	filename: 	CComFile.h
	author:		ChenQingMing
	
	purpose:	COM文件加载类
*********************************************************************/
#pragma once
#include <atlstr.h>
#include "..\CPP\tstring.h"
#include "RAII.h"

class CComFile
{
public:
	typedef HRESULT (__stdcall * FunGetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

	CComFile(const tchar* lpFileName, REFCLSID clsid);
	CComFile(REFCLSID clsid); 	

	virtual ~CComFile(void);    
public:

	virtual const tstring&	GetFileName() const { return FileName_; };
	virtual const GUID&		GetCLSID() const { return Clsid_; };

	virtual HRESULT			CreateInstance( REFIID iid, void** ppv, LPUNKNOWN pUnkOuter = NULL );
	virtual void			ReleaseFile();
    
private:
	HMODULE				ModuleHandle_;
	tstring				FileName_;
    GUID				Clsid_;
};
