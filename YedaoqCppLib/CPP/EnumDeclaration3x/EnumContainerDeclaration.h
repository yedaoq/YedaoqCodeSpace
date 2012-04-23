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
#include "EnumHeaderMicro.h"
#include "..\tstring.h"
#include "..\Enumerator.h"
#include "..\microtype.h"
#include <vector>

template<typename enumbase>
struct tagEnumItem
{
	enumbase	Val;
	tstring		ValStr;
	tstring		Description;

	tagEnumItem(enumbase val, const tchar* valstr, const tchar* desc)
		: Val(val), ValStr(valstr), Description(desc)
	{}
};

template<typename T>
tagEnumItem<T> make_enumitem(T val, const tchar* str, const tchar* desc)
{
	return tagEnumItem<T>(val, str, desc);
}

template<typename enumbase, bool flag>
class CEnumItemCollection
{
public:
	typedef tagEnumItem<enumbase>	Item;
	typedef std::vector<Item>		ItemVct;
	typedef IEnumerator<Item>		enumerator;

public:
	const Item* operator[](const tchar* valstr) const;
	const Item* operator[](enumbase val) const;

	index_t		Count() const { return Items_.size(); }
	const Item& ItemAt(index_t idx) const { return Items_[idx]; }
	void		Add(const Item& item) { Items_.push_back(item); }

	enumerator*	Enum() const;

	enumbase	Parse(const tstring& val, const tstring& separator = TEXT("|")) const;
	tstring		Format(enumbase val, const tstring& separator = TEXT("|")) const;

	tstring		GetDescription(enumbase val, const tstring& separator = TEXT("|")) const;

protected:
	ItemVct		Items_;		// list of tagEnumItem
};

template<typename T, typename enumbase = int, bool flag = false>
class CEnumContainer
{
public:
	typedef enumbase			base;
	typedef	tagEnumItem<base>	item;
	typedef IEnumerator<item>	enumerator;
	typedef CEnumItemCollection<base, flag> collection;

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

	const tstring				str() const				{return Items_.Format(Value_);}
	const tstring				desc(tchar const* separater = TEXT("|")) const {return Items_.GetDescription(Value_, separater);}

	static base					parse(const tstring& val, tchar const* separater = TEXT("|")){ return Items_.Parse(val, separater); }
	static enumerator*			items()					{return Items_.Enum();}
	static const item*			get(base			val){return Items_[val];}
	static const item*			get(const tstring&	val){return Items_[val.c_str()];}
	static const item*			get(const tchar*	val){return Items_[val];}

protected:
	base						Value_; 
	static collection			Items_;
	static items_creator		items_creator_;
};

template<typename T, typename enumbase, bool flag>
typename CEnumContainer<T,enumbase,flag>::collection CEnumContainer<T,enumbase,flag>::Items_(flag);

template<typename T, typename enumbase, bool flag>
typename CEnumContainer<T,enumbase,flag>::items_creator CEnumContainer<T,enumbase,flag>::items_creator_;

template<typename enumbase>
tstring CEnumItemCollection<enumbase, true>::Format( enumbase val, const tstring& separator ) const
{
	tstring tRet;
	bool	bFirst = true;
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->Val & val)
		{
			if(bFirst)
			{
				bFirst = false;
			}
			else
			{
				tRet.append(separator);					
			}
			tRet.append(iter->ValStr);
		}
	}

	return tRet;
}

template<typename enumbase>
tstring CEnumItemCollection<enumbase, false>::Format( enumbase val, const tstring& separator ) const
{
	tstring strRet;
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->Val == val)
		{
			strRet = iter->ValStr;
			break;
		}
	}
	return strRet;
}

template<typename enumbase>
enumbase CEnumItemCollection<enumbase, true>::Parse( const tstring& val, const tstring& separator ) const
{
	enumbase			tRet = 0;
	tstring::size_type	posPre = 0;
	tstring::size_type	posCur = 0;
	do 
	{
		posCur = val.find(separator, posPre);
		if(posCur == tstring::npos)	posCur = val.length();				
		tRet |= (*this)[val.substr(posPre, posCur - posPre)].Val;
		if(posCur >= val.length())
		{
			break;
		}
		else
		{
			posPre = posCur + separator.size();
		}

	} while (true);

	return tRet;
}

template<typename enumbase>
enumbase CEnumItemCollection<enumbase, false>::Parse( const tstring& val, const tstring& separator ) const
{
	enumbase eRet;
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->ValStr == val)
		{
			eRet = iter->Val;
			break;
		}
	}
	return eRet;
}

template<typename enumbase>
tstring CEnumItemCollection<enumbase, true>::GetDescription( enumbase val, const tstring& separator ) const
{
	tstring tRet;
	bool	bFirst = true;
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->Val & val)
		{
			if(bFirst)
			{
				bFirst = false;
			}
			else
			{
				tRet.append(separator);					
			}
			tRet.append(iter->Description);
		}
	}

	return tRet;
}

template<typename enumbase>
tstring CEnumItemCollection<enumbase, false>::GetDescription( enumbase val ) const
{
	tstring strRet;
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->Val == val)
		{
			strRet = iter->Description;
			break;
		}
	}
	return strRet;	
}

template<typename enumbase, bool flag>
const tagEnumItem<enumbase>* CEnumItemCollection<enumbase, flag>::operator[]( const tchar* valstr ) const
{
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->ValStr == valstr)
		{
			return &(*iter);
		}
	}
	return 0;
}

template<typename enumbase, bool flag>
const tagEnumItem<enumbase>* CEnumItemCollection<enumbase, flag>::operator[]( enumbase val ) const
{
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->Val == val)
		{
			return &(*iter);
		}
	}

	return 0;
}


template<typename enumbase, bool flag>
IEnumerator<tagEnumItem<enumbase>>* CEnumItemCollection<enumbase, flag>::Enum() const
{
	return new_iterator_enumerator(Items_.begin(), Items_.end());
}
