#pragma once

#define CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal) CTextConverter_##object##_##memberView##_##memberVal

#define DeclarationTextMappingConverter(object, memberView, memberVal)							\
class CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal) : public ITextFormatSwitcher		\
{																							\
public:																						\
	\
	CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal)()										\
	: m_InnerAdapter(0), m_Records(0)													\
	{}																						\
	\
	CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal)(IEnumerator<object>* records)			\
	: m_InnerAdapter(0), m_Records(records)												\
	{																						\
	ASSERT(m_Records);																	\
	}																						\
	\
	template<typename iter_t>																\
	CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal)(const iter_t& begin, const iter_t& end)\
	{																						\
	m_InnerAdapter	= new CIteratorEnumerator<iter_t>(begin, end);						\
	m_Records		= m_InnerAdapter;													\
	}																						\
	\
	CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal)(const CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal)& other)\
	{																						\
	operator=(other);																	\
	}																						\
	\
	CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal)& operator=(const CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal)& other)\
	{																						\
	if(other.m_InnerAdapter)															\
		{																					\
		m_InnerAdapter = static_cast<IEnumerator<object>*>(other.m_InnerAdapter->Clone());\
		m_Records = m_InnerAdapter;														\
		}																					\
		else																				\
		{																					\
		m_InnerAdapter = 0;																\
		m_Records = other.m_Records;													\
		}																					\
		\
		return *this;																		\
	}																						\
	\
	~CTEXTMAPPINGCONVERTERNAME(object, memberView, memberVal)()									\
	{																						\
	if(m_InnerAdapter)																	\
		{																					\
		delete m_InnerAdapter;															\
		m_InnerAdapter = 0;																\
		}																					\
	}																						\
	\
	virtual tstring Parse(const tstring& text, ICellContext* ctx)							\
	{																						\
	m_Records->Reset();																	\
	while(m_Records->MoveNext())														\
		{																					\
		if(m_Records->Current().##memberView == text)									\
			{																				\
			return m_Records->Current().##memberVal;									\
			}																				\
		}																					\
		return text;																		\
	}																						\
	\
	virtual tstring Format(LPCTSTR val, ICellContext* ctx)									\
	{																						\
	m_Records->Reset();																	\
	while(m_Records->MoveNext())														\
		{																					\
		if(m_Records->Current().##memberVal == val)										\
			{																				\
			return m_Records->Current().##memberView;									\
			}																				\
		}																					\
		return val;																			\
	}																						\
	\
protected:																					\
	\
	IEnumerator<object>*	m_InnerAdapter;													\
	IEnumerator<object>*	m_Records;														\
};														