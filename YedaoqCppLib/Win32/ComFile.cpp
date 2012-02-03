#include "StdAfx.h"
#include "ComFile.h"
#include <atlcomcli.h>

CComFile::CComFile( REFCLSID clsid, LPCTSTR lpComPath /*= 0*/, LPCTSTR lpLicence /*= 0*/ )
	: ModuleHandle_(0)
	, Clsid_(clsid)
{
	if(lpComPath) FileName_ = lpComPath;
	if(lpLicence) License = lpLicence;
}

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
		if(License.empty())
		{
			hr = CoCreateInstance( Clsid_, pUnkOuter, CLSCTX_INPROC_SERVER, iid, (void**) ppv);
		}
		else
		{
			CComPtr<IClassFactory2> spClassFactory;
			hr = CoGetClassObject(Clsid_, CLSCTX_INPROC_SERVER, NULL, __uuidof(IClassFactory2), (void**)&spClassFactory);
			if (SUCCEEDED(hr))
			{
				hr = spClassFactory->CreateInstanceLic(pUnkOuter, NULL, iid, CT2W(License.c_str()), ppv);
			}
		}
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

		if(License.empty())
		{
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
		else
		{
			CComPtr<IClassFactory2> pCF;
			if(SUCCEEDED(hr = pGetClassObject(Clsid_, __uuidof(IClassFactory2), (void**)&pCF)))
			{
				try
				{
					hr = pCF->CreateInstanceLic(pUnkOuter, NULL, iid, CT2W(License.c_str()), ppv );
				}
				catch(...)
				{
					hr = E_FAIL;
				}
			}
		}
	}
	
	return hr;
}


