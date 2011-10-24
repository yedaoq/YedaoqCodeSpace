#pragma once
#include <Singlton.h>
#include <map>
#include "DwarfViewInfo.h"
#include "DwarfViewInfoDBTable.h"
#include "LocalPlayerObjEnum.h"

class CDwarfViewProvider : public CSingleton<CDwarfViewProvider>
{
public:
	typedef std::map<int, IDwarfViewInfo*> ViewMap;

public:
	IDwarfViewInfo* operator[](int id)
	{
		ViewMap::iterator iter = Views.find(id);
		if(iter == Views.end())
		{
			_ASSERT(false);
			return 0;
		}

		return iter->second;
	}

	CDwarfViewProvider()
	{
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_CheckBytes>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_CodecPin>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Codecs>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_CodecType>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_FileInfo>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_FilterType>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_GuidInfo>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_LinkInfo>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_LinkInput>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_LinkLink>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_LinkRules>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Links>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_MediaExtension>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_MediaType>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PackageDepends>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PackageIndex>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PackageInfo>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PackageLua>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Parser>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_ParserCondition>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_ParserResult>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_PlayerKernel>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Rules>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_SourceConfig>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_SourceLink>());
		Views.insert(new CDwarfViewInfoDBTable<EnumLocalPlayerTables::TBL_Stream>());
	}

protected:
	ViewMap Views;
};