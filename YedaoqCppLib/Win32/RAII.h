#pragma once

class CRAII_DllHandle
{
public:
	CRAII_DllHandle(void)
		: Module_(0)
	{}

	CRAII_DllHandle(const tchar* dllFile)
	{
		Module_ = ::LoadLibrary(dllFile);
		if(!Module_)
		{
			DWORD dwErr = ::GetLastError();
		}
	}

	CRAII_DllHandle& operator=(CRAII_DllHandle& other)
	{
		Destroy();
		Module_ = other.Detach();
		return *this;
	}

	CRAII_DllHandle& operator=(const tchar* lpDllFile)
	{
		return CRAII_DllHandle::operator =(CRAII_DllHandle(lpDllFile));		
	}

	~CRAII_DllHandle(void)
	{
		Destroy();
	}

	HMODULE GetModule() const
	{
		return Module_;
	}

	FARPROC GetProcAddress(LPCSTR lpProcName) const
	{
		_ASSERT(Module_);
		return ::GetProcAddress(Module_, lpProcName);
	}

	HMODULE Detach()
	{
		HMODULE tmp = Module_;
		Module_ = 0;

		return tmp;
	}

	void Destroy()
	{
		if(0 != Module_)
		{
			::FreeLibrary(Module_);
			Module_ = 0;
		}
	}

protected:
	HMODULE Module_;
};

#include <comutil.h>
class CRAII_CoInit
{
public:
	CRAII_CoInit(void)
	{
		hr = CoInitialize(NULL);
	}

	CRAII_CoInit(DWORD flag)
	{
		hr = CoInitializeEx(NULL, flag);
	}

	~CRAII_CoInit(void)
	{
		if(SUCCEEDED(hr))
		{
			CoUninitialize();
		};
	}

protected:
	HRESULT	hr;
};