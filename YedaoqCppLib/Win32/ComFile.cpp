#include "StdAfx.h"
#include "ComFile.h"
#include "atlbase.h"

CComFile::CComFile( const tchar* strFileName, REFCLSID clsid  )
	: FileName_(strFileName), ModuleHandle_(0), Clsid_(clsid)
{}

CComFile::CComFile(REFCLSID clsid ): ModuleHandle_(0), Clsid_(clsid)
{}

CComFile::~CComFile(void)
{
	ReleaseFile();
}

void  CComFile::ReleaseFile()
{
	if( ModuleHandle_ )
	{
		CoFreeLibrary( ModuleHandle_ );
		ModuleHandle_ = 0;
	}
}

HRESULT CComFile::CreateInstance( REFIID iid, void** ppv, LPUNKNOWN pUnkOuter )
{
	HRESULT hr = E_FAIL;

	if(iid == GUID_NULL)
	{
		return hr;
	}

	if(FileName_.empty())
	{
		hr = CoCreateInstance( Clsid_, pUnkOuter, CLSCTX_INPROC_SERVER, iid, (void**) ppv);
	}
	else
	{
		if(!ModuleHandle_)
		{
			ModuleHandle_ = CoLoadLibrary(CComBSTR( FileName_.c_str() ), FALSE);
			if(!ModuleHandle_)
			{
				return E_FAIL;
			}
		}

		FunGetClassObject pGetClassObject = (FunGetClassObject)GetProcAddress(ModuleHandle_, "DllGetClassObject");

		if(!pGetClassObject)
		{
			return E_FAIL;
		}

		CComPtr<IClassFactory> pCF;
		if(SUCCEEDED(hr = pGetClassObject(Clsid_, __uuidof(IClassFactory), (void**)&pCF)))
		{
			try
			{
				hr = pCF->CreateInstance( pUnkOuter, iid, ppv );
			}
			catch(...)
			{
				hr = E_FAIL;
			}
		}
	}
	
	return hr;
}


