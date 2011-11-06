#pragma once

class CDynamicLinkLibrary
{
public:
	CDynamicLinkLibrary(void)
		: Module_(NULL)
	{}

	CDynamicLinkLibrary(LPCTSTR dllFile)
	{
		Module_ = ::LoadLibrary(dllFile);
		if(!Module_)
		{
			DWORD dwErr = ::GetLastError();
			dwErr = dwErr;
		}
	}

	CDynamicLinkLibrary& operator=(CDynamicLinkLibrary& other)
	{
		Destroy();
		Module_ = other.Detach();
		return *this;
	}

	CDynamicLinkLibrary& operator=(LPCTSTR lpDllFile)
	{
		return CDynamicLinkLibrary::operator =(CDynamicLinkLibrary(lpDllFile));		
	}

	~CDynamicLinkLibrary(void)
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
		Module_ = NULL;

		return tmp;
	}

	void Destroy()
	{
		if(NULL != Module_)
		{
			::FreeLibrary(Module_);
			Module_ = NULL;
		}
	}

protected:
	HMODULE Module_;
};
