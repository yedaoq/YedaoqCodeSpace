/********************************************************************
	created:	2011-6-16 16:41
	filename: 	CComFile.h
	author:		ChenQingMing
	
	purpose:	COM文件加载类
*********************************************************************/
#pragma once
#include "..\CPP\tstring.h"
#include "RAII.h"

class CComFile
{
	typedef HRESULT (__stdcall * FunGetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

public:
	CComFile(REFCLSID clsid, LPCTSTR lpComPath = 0, LPCTSTR lpLicence = 0);

	CComFile(const CComFile& other)
		: Clsid_(other.Clsid_)
		, FileName_(other.FileName_)
		, License(other.License)
		, ModuleHandle_(0)
	{}

	virtual ~CComFile(void);  

protected:
	CComFile& operator=(const CComFile&);

public:

	virtual const GUID&		GetCLSID() const { return Clsid_; };
	virtual const tstring&	GetFileName() const { return FileName_; };
	
	virtual HRESULT			CreateInstance( REFIID iid, void** ppv, LPUNKNOWN pUnkOuter = NULL );
	virtual void			ReleaseFile();
    
public:
	tstring				License;

protected:
	GUID				Clsid_;
	tstring				FileName_;
    
	HMODULE				ModuleHandle_;
};
