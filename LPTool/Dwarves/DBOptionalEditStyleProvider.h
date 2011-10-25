#pragma once
#include <Singlton.h>
#include <map>
#include <View\TextConverter4DBMap.h>
#include "DBEnumeratorProvider.h"
#include <Util\DBEnumeratorSuit.h>

class CDBOptionalEditStyleProvider : public CSingleton<CDBOptionalEditStyleProvider>
{
public:
	struct Item
	{
		int						TableID;
		int						FieldID;	

		bool operator<(const Item& other) const
		{
			int iRet = TableID - other.TableID;
			if(iRet != 0) return iRet < 0;
			return FieldID < other.FieldID;
		}
	};

	typedef std::map<Item, CEditStyleOptional> ItemMap;

public:
	CEditStyleOptional& Get(int tbl, int field)
	{
		Item item = {tbl, field};

		ItemMap::iterator iter = Items.find(item);
		if(iter == Items.end())
		{
			iter = Items.insert(
				std::make_pair(
				item, 
				CEditStyleOptional(&CDBEnumeratorProvider::GetInstance()[tbl].GetFieldEnumerator(field)))
				).first;
		}
		return iter->second;
	}

protected:
	ItemMap Items;
};