#pragma once
#include <Singlton.h>
#include <map>
#include <View\TextConverter4DBMap.h>
#include "DBEnumeratorProvider.h"
#include <Util\DBEnumeratorSuit.h>

class CDBMapFormatProvider : public CSingleton<CDBMapFormatProvider>
{
public:
	struct Item
	{
		int						TableID;
		int						FieldValue;
		int						FieldView;		

		bool operator<(const Item& other) const
		{
			int iRet = TableID - other.TableID;
			if(iRet != 0) return iRet < 0;
			iRet = FieldValue - other.FieldValue;
			if(iRet != 0) return iRet < 0;
			return FieldView < other.FieldView;
		}
	};

	typedef std::map<Item, CTextConverter4DBMap> ItemMap;

public:
	CTextConverter4DBMap& Get(int tbl, int fieidValue, int fieldView)
	{
		Item item = {tbl, fieidValue, fieldView};

		ItemMap::iterator iter = Items.find(item);
		if(iter == Items.end())
		{
			iter = Items.insert(
				std::make_pair(
					item, 
					CTextConverter4DBMap(&CDBEnumeratorProvider::GetInstance()[tbl].GetFilterEnumerator(), fieldView, fieidValue))
				).first;
		}
		return iter->second;
	}

protected:
	ItemMap Items;
};