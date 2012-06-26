#include "stdafx.h"
#include "DataTreeTest.h"
#include "DataTree/DataTreeXmlStorage.h"
#include <tchar.h>

TEST(DataTreeTemplate, All)
{
	CfgPerson me;
	me.Name = TEXT("Yedaoq");
	me.Address = TEXT("Shangi Hai");
	me.Age = 26;
	me.GoodFriend.Name = TEXT("TT");

	CfgFriend f;
	f.Name = TEXT("LJ");
	f.Ship = 60;
	me.Friends.push_back(f);
	f.Name = TEXT("SJ");
	f.Ship = 80;
	me.Friends.push_back(f);

	pugi::xml_document docA;
	nsYedaoqDataTree::CDataTreeXmlStorage ds(docA, TEXT("book"));
	me.__export(ds);
	docA.print(std::cout);

	pugi::xml_document docB;
	docB.load(TEXT("<book name=\"Abc\" address=\"Shangi Hai\" age=\"26\"><goodfriend name=\"TT\" /><friends name=\"LJ\" /><friends name=\"SJ\" /></book>"));
	nsYedaoqDataTree::CDataTreeXmlStorage dsB(docB, TEXT("book"));
	CfgPerson he;
	he.__import(dsB);
	EXPECT_EQ(he.Name, TEXT("Abc"));
}

int DataTreeTestMain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc,argv);
	int ret = RUN_ALL_TESTS();
	getchar();
	return ret;
}
