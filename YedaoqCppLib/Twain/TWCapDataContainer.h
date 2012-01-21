/* ___________________________________________________________________________
@ 
@ file - CapDataContainer.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-21
@ info -
@     
/* ___________________________________________________________________________*/
#pragma once 
#include "TWEnum.h"
#include "..\Win32\RAII.h"
#include "vector"

class CCapDataContainerBase
{
protected:
	EnumDataContainerType			ConTypeID_;
	EnumValueType					EleTypeID_;

	CCapDataContainerBase(EnumDataContainerType comType, EnumValueType eleType)
		: ConTypeID_(comType), EleTypeID_(eleType)
	{}

public:
	EnumDataContainerType			ConTypeID() const { return ConTypeID_; }
	EnumValueType					EleTypeID() const { return EleTypeID_; }

	virtual TW_HANDLE				MakeGlobalMemory() const = 0;
	static CCapDataContainerBase*	FromGlobalMemory(TW_HANDLE hGol);

public:
	template<typename TWType> TW_UINT16 TWTypeIDof() const	{ throw TWType;	}
	template<> TW_UINT16 TWTypeIDof<TW_INT8>() const { return TWTY_INT8; }
	template<> TW_UINT16 TWTypeIDof<TW_INT16>() const { return TWTY_INT16; }
	template<> TW_UINT16 TWTypeIDof<TW_INT32>() const { return TWTY_INT32; }
	template<> TW_UINT16 TWTypeIDof<TW_UINT8>() const { return TWTY_UINT8; }
	template<> TW_UINT16 TWTypeIDof<TW_UINT16>() const { return TWTY_UINT16; }
	template<> TW_UINT16 TWTypeIDof<TW_UINT32>() const { return TWTY_UINT32; }
	template<> TW_UINT16 TWTypeIDof<TW_BOOL>() const { return TWTY_BOOL; }
	template<> TW_UINT16 TWTypeIDof<TW_FRAME>() const { return TWTY_FRAME; }
	template<> TW_UINT16 TWTypeIDof<TW_FIX32>() const { return TWTY_FIX32; }
	template<> TW_UINT16 TWTypeIDof<TW_STR32>() const { return TWTY_STR32; }
	template<> TW_UINT16 TWTypeIDof<TW_STR64>() const { return TWTY_STR64; }
	template<> TW_UINT16 TWTypeIDof<TW_STR128>() const { return TWTY_STR128; }
	template<> TW_UINT16 TWTypeIDof<TW_STR255>() const { return TWTY_STR255; }
};

template<typename eletype>
class CCapDataContainerOneValue : public CCapDataContainerBase
{
public:
	eletype Value;

	CCapDataContainerOneValue(eletype val)
		: CCapDataContainerBase(EnumDataContainerType::ONEVALUE, TWTypeIDof(eletype))
		, Value(val)
	{}	

	virtual TW_HANDLE MakeGlobalMemory() const;
};

template<typename eletype>
class CCapDataContainerRange : public CCapDataContainerBase
{
public:
	TW_UINT32  MinValue;     /* Starting value in the range.           */
	TW_UINT32  MaxValue;     /* Final value in the range.              */
	TW_UINT32  StepSize;     /* Increment from MinValue to MaxValue.   */
	TW_UINT32  DefaultValue; /* Power-up value.                        */
	TW_UINT32  CurrentValue; /* The value that is currently in effect. */

	CCapDataContainerRange(TW_UINT32 min = 0, TW_UINT32 max = 0, TW_UINT32 cur = 0, TW_UINT32 def = 0, TW_UINT32 step = 1)
		: CCapDataContainerBase(EnumDataContainerType::RANGE, TWTypeIDof(eletype))
		, MinValue(min), MaxValue(max), StepSize(step), DefaultValue(def), CurrentValue(cur)
	{}

	virtual TW_HANDLE MakeGlobalMemory() const;
};


template<typename eletype>
class CCapDataContainerArray : public CCapDataContainerBase, public std::vector<eletype>
{
public:
	CCapDataContainerArray()
		: CCapDataContainerBase(EnumDataContainerType::ARRAY, TWTypeIDof(eletype))
	{}

	virtual TW_HANDLE MakeGlobalMemory() const;
};

template<typename eletype>
class CCapDataContainerEnumeration : public CCapDataContainerArray<eletype>
{
public:
	TW_UINT32	CurrentIndex; 
	TW_UINT32	DefaultIndex; 

	CCapDataContainerEnumeration()
		: CCapDataContainerBase(EnumDataContainerType::ENUM, TWTypeIDof(eletype))
	{}

	virtual TW_HANDLE MakeGlobalMemory() const;
};

template<typename eletype>
TW_HANDLE CCapDataContainerOneValue<eletype>::MakeGlobalMemory() const
{
	CRAII_GlobalMemoryHandle global(sizeof(TW_ONEVALUE) - sizeof(TW_ONEVALUE.Item) + sizeof(eletype));

	TW_ONEVALUE *pData = global.Lock<TW_ONEVALUE>();
	pData->ItemType = EleTypeID_;
	*(static<eletype*>(&pData->Item)) = Value;

	global.Unlock();
	return global.Detach();
}

template<typename eletype>
TW_HANDLE CCapDataContainerRange<eletype>::MakeGlobalMemory() const
{
	CRAII_GlobalMemoryHandle global(sizeof(TW_RANGE));

	TW_RANGE *pData = global.Lock<TW_RANGE>();
	pData->ItemType = EleTypeID_;
	pData->MaxValue = MaxValue;
	pData->MinValue = MinValue;
	pData->StepSize = StepSize;
	pData->CurrentValue = CurrentValue;
	pData->DefaultValue = DefaultValue;

	global.Unlock();
	return global.Detach();
}

template<typename eletype>
TW_HANDLE CCapDataContainerArray<eletype>::MakeGlobalMemory() const
{
	CRAII_GlobalMemoryHandle global(sizeof(TW_ARRAY) - sizeof(TW_ARRAY.ItemList) + sizeof(eletype) * size());

	TW_ARRAY *pData = global.Lock<TW_ARRAY>();
	pData->ItemType = EleTypeID_;
	pData->NumItems = size();
	copy(begin(), end(), (eletype*)pData->ItemList);

	global.Unlock();
	return global.Detach();
}

template<typename eletype>
TW_HANDLE CCapDataContainerEnumeration<eletype>::MakeGlobalMemory() const
{
	CRAII_GlobalMemoryHandle global(sizeof(TW_ENUMERATION) - sizeof(TW_ENUMERATION.ItemList) + sizeof(eletype) * size());

	TW_ENUMERATION *pData = global.Lock<TW_ENUMERATION>();
	pData->ItemType = EleTypeID_;
	pData->NumItems = size();
	pData->CurrentIndex = CurrentIndex;
	pData->DefaultIndex = DefaultIndex;
	copy(begin(), end(), (eletype*)pData->ItemList);

	global.Unlock();
	return global.Detach();
}
