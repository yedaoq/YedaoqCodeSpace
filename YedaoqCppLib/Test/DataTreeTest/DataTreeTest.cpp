// DataTreeTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include "CPP/tstring.h"
#include "Types.h"
#include "CPP/DataTree/DataTreeXmlStorage.h"

using namespace nsYedaoqDataTree;

int _tmain(int argc, _TCHAR* argv[])
{
	CfgBook book;
	book.ID = 1;
	book.Ended = true;
	book.Name = TEXT("舞动乾坤");

	std::vector<CfgChapter> chapters;
	CfgChapter chapter;
	chapter.Idx = 0;
	chapter.Title = TEXT("第一章 A");
	chapters.push_back(chapter);

	chapter.Idx = 1;
	chapter.Title = TEXT("第二章 B");
	chapters.push_back(chapter);

	chapter.Idx = 2;
	chapter.Title = TEXT("第三章 C");
	chapters.push_back(chapter);

	chapter.Idx = 3;
	chapter.Title = TEXT("第四章 D");
	chapters.push_back(chapter);

	chapter.Idx = 4;
	chapter.Title = TEXT("第五章 E");
	chapters.push_back(chapter);

	CfgSourceBook source_book;
	source_book.SourceID = 1;
	source_book.Chapters = chapters;
	source_book.SourceBookID= 232342;
	source_book.CurChapter = chapters[2];
	book.Sources.push_back(source_book);

	source_book.SourceID = 2;
	source_book.SourceBookID= 232332;
	source_book.CurChapter = chapters[3];
	book.Sources.push_back(source_book);

	source_book.SourceID = 3;
	source_book.SourceBookID= 232345;
	source_book.CurChapter = chapters[4];
	book.Sources.push_back(source_book);

	nsYedaoqDataTree::CXmlFile<CfgBook> f(TEXT("C:\\test.xml"), TEXT("book"));
	f.Save(book);

	CfgBook bookB;
	f.Load(bookB);

	bookB.Name = TEXT("遮天");
	bookB.Sources[1].Chapters[2].Title = TEXT("第三章 G");

	nsYedaoqDataTree::CXmlFile<CfgBook> fb(TEXT("C:\\testB.xml"), TEXT("book"));
	fb.Save(bookB);

	return 0;
}
