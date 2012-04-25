/* ___________________________________________________________________________
@ 
@ file - EnumContainerDeclaration.cpp
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-19
@ info -
@     
/* ___________________________________________________________________________*/
#pragma once
#include "..\tstring.h"
#include "..\Enumerator.h"
#include "..\Singlton.h"
#include "..\microtype.h"
#include <vector>


namespace nsYedaoqEnum
{
	template<typename enumbase>
	struct tagEnumItem
	{
		enumbase	Val;
		tstring		ValStr;
		tstring		Desc;

		tagEnumItem(enumbase val, const tchar* valstr, const tchar* desc)
			: Val(val), ValStr(valstr), Desc(desc)
		{}

		tagEnumItem(){}
	};

	template<typename enumbase>
	struct enum_collectiontraits
	{
		typedef std::vector<tagEnumItem<enumbase> > collection_t;
	};

	template<typename T>
	tagEnumItem<T> make_enumitem(T val, const tchar* str, const tchar* desc)
	{
		return tagEnumItem<T>(val, str, desc);
	};

	template<typename T>
	tagEnumItem<T> const* find_enumitem(T val, typename enum_collectiontraits<T>::collection_t const& items)
	{
		tagEnumItem<T> const* pRet = 0;
		STDCENUM(typename enum_collectiontraits<T>::collection_t, items, iter)
		{
			if(iter->Val == val)
			{
				pRet = &(*iter);
			}
		}
		return pRet;
	}

	template<typename T>
	tagEnumItem<T> const* find_enumitem(tchar const* str, typename enum_collectiontraits<T>::collection_t const& items)
	{
		tagEnumItem<T> const* pRet = 0;
		STDCENUM(typename enum_collectiontraits<T>::collection_t, items, iter)
		{
			if(iter->ValStr == str)
			{
				pRet = &(*iter);
			}
		}
		return pRet;
	}

	template<typename T>
	tagEnumItem<T> const* find_enumitem(tstring const& str, typename enum_collectiontraits<T>::collection_t const& items)
	{
		tagEnumItem<T> const* pRet = 0;
		STDCENUM(std::vector<tagEnumItem<T>>, items, iter)
		{
			if(iter->ValStr == str)
			{
				pRet = &(*iter);
			}
		}
		return pRet;
	}

	template<typename enumbase, bool Flag>
	struct enum_flagtraits{};

	template<typename enumbase>
	struct enum_flagtraits<enumbase, true>
	{
		typedef tstring str_t;

		static str_t Format(enumbase v, typename enum_collectiontraits<enumbase>::collection_t const& items, tchar const* separater = TEXT(" | "))
		{
			tstring tRet;
			bool	bFirst = true;
			STDCENUM(enum_collectiontraits<enumbase>::collection_t, items, iter)
			{
				if(iter->Val & v)
				{
					if(bFirst)
					{
						bFirst = false;
					}
					else
					{
						tRet.append(separater);					
					}
					tRet.append(iter->ValStr);
				}
			}

			return tRet;
		}

		static str_t GetDesc(enumbase v, typename enum_collectiontraits<enumbase>::collection_t const& items, tchar const* separater = TEXT(" | "))
		{
			tstring tRet;
			bool	bFirst = true;
			STDCENUM(enum_collectiontraits<enumbase>::collection_t, items, iter)
			{
				if(iter->Val & v)
				{
					if(bFirst)
					{
						bFirst = false;
					}
					else
					{
						tRet.append(separater);					
					}
					tRet.append(iter->Desc);
				}
			}

			return tRet;
		}

		static enumbase Parse( const tstring& valstr, typename enum_collectiontraits<enumbase>::collection_t const& items, tstring const& separater = TEXT("|") )
		{
			enumbase			tRet = 0;
			tstring::size_type	posPre = 0;
			tstring::size_type	posCur = 0;
			do 
			{
				posCur = valstr.find(separater, posPre);
				if(posCur == tstring::npos)	posCur = valstr.length();

				if(posCur != posPre)
				{
					tagEnumItem<enumbase>* item = find_enumitem(valstr.substr(posPre, posCur - posPre), items);
					if(item)
					{
						tRet |= item->Val;
					}
				}				

				if(posCur >= valstr.length())
				{
					break;
				}
				else
				{
					posPre = posCur + separater.length();
				}

			} while (true);

			return tRet;
		}
	};

	template<typename enumbase>
	struct enum_flagtraits<enumbase, false>
	{
		typedef tstring const& str_t;

		static str_t Format(enumbase v, typename enum_collectiontraits<enumbase>::collection_t const& items, tchar const* separater = 0)
		{
			tagEnumItem<enumbase> const* item = find_enumitem(v, items);
			return item ? item->ValStr : CSingleton<const tstring>::GetInstance();
		}

		static str_t GetDesc(enumbase v, typename enum_collectiontraits<enumbase>::collection_t const& items, tchar const* separater = 0)
		{
			tagEnumItem<enumbase> const* item = find_enumitem(v, items);
			return item ? item->Desc : CSingleton<const tstring>::GetInstance();
		}

		static enumbase Parse(const tstring& valstr, typename enum_collectiontraits<enumbase>::collection_t const& items, tchar const* separater = 0 )
		{
			tagEnumItem<enumbase> const* item = find_enumitem(valstr, items);
			return item ? item->Val : 0;
		}
	};

	template<typename T, typename enumbase = int, bool flag = false>
	class CEnumContainer
	{
	public:
		typedef enumbase			base;
		typedef	tagEnumItem<base>	item;
		typedef IEnumerator<item>	enumerator;
		typedef typename enum_collectiontraits<base>::collection_t collection;

	protected:
		struct items_creator 
		{
			items_creator()
			{
				T::Initialize();
			}
			inline void do_nothing() const {}
		};

		CEnumContainer(){ }

	public:

		CEnumContainer(base v) : Value_(v) {}
		CEnumContainer(CEnumContainer const& v) : Value_(v.Value_) {}

		CEnumContainer& operator=(base v) { Value_ = v; return *this; }
		CEnumContainer& operator=(CEnumContainer const& v) { Value_ = v.Value_; return *this; }

		operator base() const { return Value_; }
		bool operator==(base v) const { return v == Value_; }
		bool operator!=(base v) const { return v != Value_; }

		void operator&=(base v) { Value_ &= v; }
		void operator|=(base v) { Value_ |= v; }
		void operator^=(base v) { Value_ ^= v; }

		typename enum_flagtraits<enumbase, flag>::str_t	str(tchar const* separater = TEXT("|")) const {return enum_flagtraits<enumbase, flag>::Format(Value_, Items_, separater);}
		typename enum_flagtraits<enumbase, flag>::str_t	desc(tchar const* separater = TEXT("|")) const {return enum_flagtraits<enumbase, flag>::GetDesc(Value_, Items_, separater);}

		static base					parse(const tstring& val, tchar const* separater = TEXT("|")){ return enum_flagtraits<enumbase, flag>::Parse(val, Items_, separater); }
		
		static collection&			items()					{return Items_;}
		static const item*			get(base			val){return find_enumitem<base>(val, Items_);}
		static const item*			get(const tstring&	val){return find_enumitem<base>(val, Items_);}
		static const item*			get(const tchar*	val){return find_enumitem<base>(val, Items_);}

	protected:
		base						Value_; 
		static collection			Items_;
		static items_creator		items_creator_;
	};

	template<typename T, typename enumbase, bool flag>
	typename CEnumContainer<T,enumbase,flag>::collection CEnumContainer<T,enumbase,flag>::Items_(0);

	template<typename T, typename enumbase, bool flag>
	typename CEnumContainer<T,enumbase,flag>::items_creator CEnumContainer<T,enumbase,flag>::items_creator_;
}
