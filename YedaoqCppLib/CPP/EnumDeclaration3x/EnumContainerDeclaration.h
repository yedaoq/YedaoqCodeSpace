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

template<typename enumbase>
class CEnumItemCollection
{
public:
	typedef tagEnumItem<enumbase>	Item;
	typedef std::vector<Item>		ItemVct;
	typedef IEnumerator<Item>		enumerator;

public:
	CEnumItemCollection(bool flags)
		: Flag_(flags)
	{}

	const Item& operator[](const tchar* valstr) const;
	const Item& operator[](enumbase val) const;

	index_t		Count() const { return Items_.size(); }
	const Item& ItemAt(index_t idx) const { return Items_[idx]; }
	void		Add(const Item& item) { Items_.push_back(item); }

	enumerator*	Enum() const;

	enumbase	Parse(const tstring& val, const tstring& separator = TEXT("|")) const;
	tstring		Format(enumbase val, const tstring& separator = TEXT("|")) const;

	tstring		GetDescription(enumbase val, const tstring& separator = TEXT("|")) const;

protected:

	enumbase	ParseNormalEnum(const tstring& val) const;
	tstring		FormatNormalEnum(enumbase val) const;

	enumbase	ParseFlagsEnum(const tstring& val, const tstring& separator) const;
	tstring		FormatFlagsEnum(enumbase val, const tstring& separator) const;

	tstring		GetNormalEnumDescription(enumbase val) const;
	tstring		GetFlagsEnumDescription(enumbase val, const tstring& separator) const;

protected:
	ItemVct		Items_;		// list of tagEnumItem
	bool		Flag_;		// identify that whether the enum is a flags enum or not
};

template<typename T, typename enumbase = int, bool flag = false>
class CEnumContainer
{
public:
	typedef enumbase			base;
	typedef	tagEnumItem<base>	item;
	typedef IEnumerator<item>	enumerator;
	typedef CEnumItemCollection<base> collection;

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

	const tstring				str() const				{return Items_.Format(Value_);}
	const tstring				desc() const			{return Items_.GetDescription(Value_);}

	static enumerator*			items()					{return Items_.Enum();}
	static const item*			get(base			val){return Items_[val];}
	static const item*			get(const tstring&	val){return Items_[val.c_str()];}
	static const item*			get(const tchar*	val){return Items_[val];}

protected:
	base						Value_; 
	static collection			Items_;
	static items_creator		items_creator_;
};

template<typename T, typename enumbase /*= int*/, bool flag /*= false*/>
typename CEnumContainer<T,enumbase,flag>::collection CEnumContainer<T,enumbase,flag>::Items_(flag);

template<typename T, typename enumbase /*= int*/, bool flag /*= false*/>
typename CEnumContainer<T,enumbase,flag>::items_creator CEnumContainer<T,enumbase,flag>::items_creator_;

template<typename enumbase>
tstring CEnumItemCollection<enumbase>::Format( enumbase val, const tstring& separator /*= TEXT("|")*/ ) const
{
	if(Flag_)
	{
		return FormatFlagsEnum(val, separator);
	}
	else
	{
		return FormatNormalEnum(val/*, separator*/);
	}
}

template<typename enumbase>
enumbase CEnumItemCollection<enumbase>::Parse( const tstring& val, const tstring& separator /*= TEXT("|")*/ ) const
{
	if(Flag_)
	{
		return ParseFlagsEnum(val/*, separator*/);
	}
	else
	{
		return ParseNormalEnum(val/*, separator*/);
	}
}


template<typename enumbase>
tstring CEnumItemCollection<enumbase>::GetDescription( enumbase val, const tstring& separator /*= TEXT("|")*/ ) const
{
	if(Flag_)
	{
		return GetFlagsEnumDescription(val, separator);
	}
	else
	{
		return GetNormalEnumDescription(val);
	}
}

template<typename enumbase>
tstring CEnumItemCollection<enumbase>::FormatNormalEnum( enumbase val ) const
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
enumbase CEnumItemCollection<enumbase>::ParseNormalEnum( const tstring& val ) const
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
tstring CEnumItemCollection<enumbase>::FormatFlagsEnum( enumbase val, const tstring& separator /*= TEXT("|")*/ ) const
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
enumbase CEnumItemCollection<enumbase>::ParseFlagsEnum( const tstring& val, const tstring& separator /*= TEXT("|")*/ ) const
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
tstring CEnumItemCollection<enumbase>::GetFlagsEnumDescription( enumbase val, const tstring& separator ) const
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
tstring CEnumItemCollection<enumbase>::GetNormalEnumDescription( enumbase val ) const
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

template<typename enumbase>
const tagEnumItem<enumbase>& CEnumItemCollection<enumbase>::operator[]( const tchar* valstr ) const
{
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->ValStr == valstr)
		{
			return *iter;
		}
	}
	throw std::exception();
}

template<typename enumbase>
const tagEnumItem<enumbase>& CEnumItemCollection<enumbase>::operator[]( enumbase val ) const
{
	STDCENUM(ItemVct, Items_, iter)
	{
		if(iter->Val == val)
		{
			return *iter;
		}
	}

	throw std::exception();
}


template<typename enumbase>
IEnumerator<tagEnumItem<enumbase>>* CEnumItemCollection<enumbase>::Enum() const
{
	return new_iterator_enumerator(Items_.begin(), Items_.end());
}
