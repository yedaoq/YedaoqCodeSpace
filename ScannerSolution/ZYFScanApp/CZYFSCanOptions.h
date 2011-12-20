#pragma once

#include "mytype.h"
#include "Profile.h"
#include "..\..\YedaoqCodeSpace\XMLite Demo\XMLite.h"
#include "Helper.h"
#include "fstream"

struct ScanItem
{
	tstring Name;
	CPoint	ScanPos;
	CSize	ScanSize;
	CPoint	ShowPos;

	bool Serialize(XNode& node)
	{
		node.name = TEXT("ScanItem");
		node.AppendAttr(TEXT("ItemName"), Name.c_str());
		node.AppendAttr(TEXT("ScanPos"), ToStr(ScanPos).c_str());
		node.AppendAttr(TEXT("ScanSize"), ToStr(ScanSize).c_str());
		node.AppendAttr(TEXT("ShowPos"), ToStr(ShowPos).c_str());

		return true;
	}

	bool Unserialize(XNode& node)
	{
		ASSERT(node.name == TEXT("ScanItem"));
		Name = node.GetAttrValue(TEXT("ItemName"));
		ScanPos = FromStr<CPoint>(node.GetAttrValue(TEXT("ScanPos")));
		ScanSize = FromStr<CSize>(node.GetAttrValue(TEXT("ScanSize")));
		ShowPos = FromStr<CPoint>(node.GetAttrValue(TEXT("ShowPos")));

		return true;
	}
};

class CScanBatch
{
public:
	tstring Name;
	std::vector<ScanItem> Items;

	bool Serialize(XNode& node)
	{
		XNode* item;
		node.name = TEXT("ScanBatch");
		node.AppendAttr(TEXT("BatchName"), Name.c_str());
		for (int i = 0; i < Items.size(); ++i)
		{
			item = new XNode;
			Items[i].Serialize(*item);
			node.AppendChild(item);
		}

		return true;
	}

	bool Unserialize(XNode& node)
	{
		if(node.name != TEXT("ScanBatch"))
		{
			ASSERT(false);
			return false;
		}

		Name = node.GetAttrValue(TEXT("BatchName"));

		Items.clear();
		ScanItem item;
		for (int i = 0; i < node.GetChildCount(); ++i)
		{
			if(node.GetChild(i)->name == TEXT("ScanItem"))
			{
				item.Unserialize(*node.GetChild(i));
				Items.push_back(item);
			}
		}

		return true;
	}
};

class CZYFScanOptions : public CSingleton<CZYFScanOptions>
{
public:
	std::vector<CScanBatch> ScanBatchs;
	CSize					DocSize;

public:
	void Save()
	{
		TCHAR szBuf[MAX_PATH];
		::GetModuleFileName(NULL, szBuf, MAX_PATH);
		PathRemoveFileSpec(szBuf);
		PathAppend(szBuf, TEXT("config.ini"));

		XNode root;
		Serialize(root);

		std::locale prev_loc = std::locale::global( std::locale("chs") );
		std::tofstream f(szBuf, std::ios_base::out);
		CString str = root.GetXML();
		//f<<(LPCTSTR)root.GetXML();
		f.write(str, str.GetLength());
		f.flush();
		f.close();
	}

	void Load()
	{
		TCHAR szBuf[MAX_PATH];
		::GetModuleFileName(NULL, szBuf, MAX_PATH);
		PathRemoveFileSpec(szBuf);
		PathAppend(szBuf, TEXT("config.ini"));

		XNode root;
		
		std::locale prev_loc = std::locale::global( std::locale("chs") );
		std::tifstream f(szBuf, std::ios_base::in);
		//CFile f(szBuf, CFile::modeCreate | CFile::modeRead);
		TCHAR szCom[65535] = {0};
		f.getline(szCom, 65534, EOF);/*'{reply.content}'*/
		root.Load(szCom);
		Unserialize(root);
		f.close();
	}

	bool Serialize(XNode& node)
	{
		XNode* item;
		node.name = TEXT("ZYFScanAppSetting");
		for (int i = 0; i < ScanBatchs.size(); ++i)
		{
			item = new XNode;
			ScanBatchs[i].Serialize(*item);
			node.AppendChild(item);
		}

		return true;
	}

	bool Unserialize(XNode& node)
	{
		if(node.name != TEXT("ZYFScanAppSetting"))
		{
			ASSERT(false);
			return false;
		}

		CScanBatch item;
		ScanBatchs.clear();
		for (int i = 0; i < node.GetChildCount(); ++i)
		{
			if(node.GetChild(i)->name == TEXT("ScanBatch"))
			{
				item.Unserialize(*node.GetChild(i));
				ScanBatchs.push_back(item);
			}
		}

		return true;
	}
};