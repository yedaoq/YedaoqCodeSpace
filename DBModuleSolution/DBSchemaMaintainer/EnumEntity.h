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

	T&	operator[](int i)
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
	bool swap(T& lhs, T& rhs)
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

struct ENUMITEM
{
	int		ValueInt;
	tstring ValueStr;
	tstring ValueDescription;
};

class ENUMENTITY
{
public:
	ENUMITEM& operator[](const tchar* valstr)
	{
		for(int i = 0; i < Items_.Count(); ++i)
		{
			if(Items_[i].ValueStr == valstr)
			{
				return Items_[i];
			}
		}
	}

	ENUMITEM& operator[](int val)
	{
		for(int i = 0; i < Items_.Count(); ++i)
		{
			if(Items_[i].ValueInt == val)
			{
				return Items_[i];
			}
		}
	}

	bool Add(const ENUMITEM& item)
	{
		return Items_.Add(item);
	}

public:
	DynamicArray<ENUMITEM> Items_;
};

//#define BEGINENUM(name) enum Enum##name {
#define BEGINENUM(name,comment) enum Enum##name{ /##/##/ ##comment
#define ENDENUM };
//#define ENUMITEM(name) name,
//#define ENUMITEM(name, comment) name, // comment
#define ENUMITEM(name, comment, val) name = val, // ##comment