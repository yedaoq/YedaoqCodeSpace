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

	T&	operator[](int i) const
	{
		_ASSERT(i >= 0 && i < Count_);
		return Items_[i];
	}

	int Count() const {return Count_; }

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
	bool ChangeCapacity(int iCapacity)
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
	int			Count_;
	int			Capacity_;
};

template<typename T>
struct ENUMITEM
{
	T		ValueEnum;
	tstring ValueStr;
	tstring ValueDescription;

	ENUMITEM(T val, const tstring& str, const tstring& desc)
		: ValueEnum(val), ValueStr(str), ValueDescription(desc)
	{}
};

template<typename T>
class ENUMENTITY
{
public:
	const ENUMITEM<T>& operator[](const tchar* valstr) const
	{
		for(int i = 0; i < Items_.Count(); ++i)
		{
			if(Items_[i].ValueStr == valstr)
			{
				return Items_[i];
			}
		}
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
	}

	bool Add(const ENUMITEM<T>& item)
	{
		return Items_.Add(item);
	}

public:
	DynamicArray<ENUMITEM<T>> Items_;
};

#ifndef ENUMNAME
#define ENUMNAME(name) Enum##name
#endif

#ifndef BEGINENUM
#define BEGINENUM(name,comment) enum ENUMNAME(name); \
	const ENUMENTITY<ENUMNAME(name)>& EnumEntityOf##name();  \
	enum ENUMNAME(name){ 
#endif

#ifndef ENDENUM
#define ENDENUM };
#endif

#ifndef ENUMITEM0
#define ENUMITEM0(name) name,
#endif

#ifndef ENUMITEM1
#define ENUMITEM1(name, desc) name, 
#endif

#ifndef ENUMITEM2
#define ENUMITEM2(name, val) name = val, 
#endif

#ifndef ENUMITEM3
#define ENUMITEM3(name, val, desc) name = val,
#endif



