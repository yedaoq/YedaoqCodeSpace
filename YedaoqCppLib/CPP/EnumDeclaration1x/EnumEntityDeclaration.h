#pragma once
#include "mytype.h"

template<typename T>
class DynamicArray
{
public:
	DynamicArray(int capacity = 8)
		: Count_(0), Items_(0)
	{
		ChangeCapacity(capacity);
	}

	~DynamicArray()
	{
		if(Items_)
		{
			delete[] Items_;
			Items_ = 0;
			Count_ = 0;
			Capacity_ = 0;
		}
	}

	T&	operator[](index_t i) const
	{
		if(i < 0 || i >= Count_)
		{
			throw ERROR_INVALID_ACCESS;
			//_ASSERT(false);
			throw std::exception();
		}
		return Items_[i];
	}

	index_t Count() const {return Count_; }

	bool Add(const T& item)
	{
		if(Count_ >= Capacity_)
		{
			if(!ChangeCapacity(Capacity_ * 2) || Count_ >= Capacity_)
			{
				return false;
			}
		}

		Items_[Count_++] = item;
		return true;
	}

protected:
	bool ChangeCapacity(index_t iCapacity)
	{
		_ASSERT(iCapacity >= 0);
		T* tmp = new T[iCapacity];
		if(tmp && swap(tmp, Items_))
		{
			if(tmp)
			{
				delete[] tmp;
				tmp = 0;
			}
			Capacity_ = iCapacity;
			return true;
		}

		return false;
	}

	template<typename T>
	bool swap(T& lhs, T& rhs) const
	{
		T tmp = lhs;
		lhs = rhs;
		rhs = tmp;
		return true;
	}

protected:
	T*			Items_;
	index_t		Count_;
	index_t		Capacity_;
};

template<typename T>
struct ENUMITEM
{
	T		ValueEnum;
	tstring ValueStr;
	tstring ValueDescription;

	template<typename T> T Value() const { return (T)ValueEnum; }

	ENUMITEM(){}

	ENUMITEM(T val, const tstring& str, const tstring& desc)
		: ValueEnum(val), ValueStr(str), ValueDescription(desc)
	{}
};

template<typename T>
ENUMITEM<T> make_enumitem(T val, const tstring& str, const tstring desc)
{
	return ENUMITEM<T>(val, str, desc);
}

template<typename T>
class ENUMENTITY
{
public:

	index_t Count() const { return Items_.Count(); }

	const ENUMITEM<T>& ItemAt(index_t idx) const { return Items_[idx]; }

	const ENUMITEM<T>& operator[](const tchar* valstr) const
	{
		for(int i = 0; i < Items_.Count(); ++i)
		{
			if(Items_[i].ValueStr == valstr)
			{
				return Items_[i];
			}
		}
		throw std::exception();
	}

	const ENUMITEM<T>& operator[](T val) const
	{
		for(int i = 0; i < Items_.Count(); ++i)
		{
			if(Items_[i].ValueEnum == val)
			{
				return Items_[i];
			}
		}
		throw std::exception();
	}

	const ENUMITEM<T>& operator[](const tstring& valstr) const { return (*this)[valstr.c_str()]; }

	bool Add(const ENUMITEM<T>& item)
	{
		return Items_.Add(item);
	}

	T ParseFlagEnum(const tstring& val, const tstring& separator = TEXT("|")) const
	{
		T tRet = 0;
		tstring::size_type posPre = 0;
		tstring::size_type posCur = 0;
		do 
		{
			posCur = val.find(separator, posPre);
			if(posCur == tstring::npos)	posCur = val.length();				
			tRet |= (*this)[val.substr(posPre, posCur - posPre)].ValueEnum;
			if(posCur == val.length())
			{
				break;
			}
			else
			{
				posPre = posCur + 1;
			}

		} while (true);

		return tRet;
	}

	tstring FormatFlagEnum(T val, const tstring& separator = TEXT("|")) const
	{
		tstring tRet;
		bool	bFirst = true;
		for (index_t i = 0; i < Count(); ++i)
		{
			if(ItemAt(i).ValueEnum & val)
			{
				if(bFirst)
				{
					bFirst = false;
				}
				else
				{
					tRet.append(separator);					
				}
				tRet.append(ItemAt(i).ValueStr);
			}
		}

		return tRet;
	}

public:
	DynamicArray<ENUMITEM<T>> Items_;
};

template<typename T>
static T ParseFlagEnum(const ENUMENTITY<T>& entity, const tstring& val, const tstring& separator = TEXT("|"))
{
	T tRet = 0;
	tstring::size_type posPre = 0;
	tstring::size_type posCur = 0;
	do 
	{
		posCur = val.find(separator, posPre);
		if(posCur == tstring::npos)	posCur = val.length();				
		tRet |= entity[val.substr(posPre, posCur - posPre)].ValueEnum;
		if(posCur == val.length())
		{
			break;
		}
		else
		{
			posPre = posCur + 1;
		}

	} while (true);

	return tRet;
}

template<typename T>
static tstring FormatFlagEnum(const ENUMENTITY<T>& entity, T val, const tstring& separator = TEXT("|"))
{
	tstring tRet;
	bool	bFirst = true;
	for (index_t i = 0; i < entity.Count(); ++i)
	{
		if(entity.ItemAt(i).ValueEnum & val)
		{
			if(!bFirst)
			{
				tRet.append(separator);
				bFirst = true;
			}
			tRet.append(entity.ItemAt(i).ValueStr);
		}
	}

	return tRet;
}
