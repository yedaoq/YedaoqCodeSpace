#pragma once
//#include <tchar.h>

#define RETURN_ASSERTFAIL(ret,expr) if(!(expr)){ ASSERT(false); return ret; }
#define RETURN_FAIL(ret,expr) if(!(expr)){ return ret; }
#define RETURN0_ASSERTFAIL(expr) if(!(expr)){ ASSERT(false); return; }
#define RETURN0_FAIL(expr) if(!(expr)){ return; }

void WTRACE(wchar_t const* pszFmt, ...);

#ifdef UNICODE
#define TTRACE WTRACE
#else
#define TTRACE TRACE
#endif

namespace NSYedaoqTrace
{
	interface ITraceReceiver
	{
		virtual void Receive(DWORD_PTR dwModule,const char *pszFileName, int nLineNo,
			UINT nCategory, UINT nLevel, const WCHAR *pszFormat, va_list ptr)
	};

	struct TraceCategory
	{
		unsigned int	ID;
		tstring			Name;
		unsigned int	Parent;
	};

	interface ITraceCategoryProvider
	{
		virtual const TraceCategory& operator[](unsigned int id) const;
		virtual TraceCategory& operator[](unsigned int id);
	};

	class CTraceControl
	{
	public:
		typedef std::auto_ptr<ITraceCategoryProvider> CategoryProviderPtr;

	public:
		CTraceControl();

		void Dispatch(const char *pszFileName, int nLineNo,	UINT nCategory, UINT nLevel, const WCHAR *pszFormat, va_list ptr);
		void Dispatch(const char *pszFileName, int nLineNo,	UINT nCategory, UINT nLevel, const CHAR *pszFormat, va_list ptr);

		

		

	public:
		static CTraceControl	Instance;
		static DWORD			Pid;

		UINT					CategoryDefault;
		UINT					LevelDefault;
		UINT					LevelThreshold;
		CategoryProviderPtr		Provider;
	};

	class CTraceFileAndLineInfo
	{
	public:
		CTraceFileAndLineInfo(const char *pszFileName, int nLineNo)
			: m_pszFileName(pszFileName), m_nLineNo(nLineNo)
		{}

#pragma warning(push)
#pragma warning(disable : 4793)
		void __cdecl operator()(DWORD_PTR dwCategory, UINT nLevel, const char *pszFmt, ...) const
		{
			va_list ptr; va_start(ptr, pszFmt);
			ATL::CTrace::s_trace.TraceV(m_pszFileName, m_nLineNo, dwCategory, nLevel, pszFmt, ptr);
			va_end(ptr);
		}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4793)
		void __cdecl operator()(DWORD_PTR dwCategory, UINT nLevel, const wchar_t *pszFmt, ...) const
		{
			va_list ptr; va_start(ptr, pszFmt);
			ATL::CTrace::s_trace.TraceV(m_pszFileName, m_nLineNo, dwCategory, nLevel, pszFmt, ptr);
			va_end(ptr);
		}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4793)
		void __cdecl operator()(const char *pszFmt, ...) const
		{
			va_list ptr; va_start(ptr, pszFmt);
			ATL::CTrace::s_trace.TraceV(m_pszFileName, m_nLineNo, atlTraceGeneral, 0, pszFmt, ptr);
			va_end(ptr);
		}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4793)
		void __cdecl operator()(const wchar_t *pszFmt, ...) const
		{
			va_list ptr; va_start(ptr, pszFmt);
			ATL::CTrace::s_trace.TraceV(m_pszFileName, m_nLineNo, atlTraceGeneral, 0, pszFmt, ptr);
			va_end(ptr);
		}
#pragma warning(pop)

	private:
		/* unimplemented */
		CTraceFileAndLineInfo &__cdecl operator=(const CTraceFileAndLineInfo &right);

		const char *const m_pszFileName;
		const int m_nLineNo;
	};
}

