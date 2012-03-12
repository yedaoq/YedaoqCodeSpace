#include "stdafx.h"
#include "EnumTest.h"
#include <tchar.h>

TEST(EnumTemplate2, VAL)
{
	EnumTest e = EnumTest::A;
	EXPECT_EQ(0, e);

	e = EnumTest::B;
	EXPECT_EQ(1, e);

	e = EnumTest::C;
	EXPECT_EQ(5, e);

	e = EnumTest::D;
	EXPECT_EQ(7, e);

	e = EnumTest::E;
	EXPECT_EQ(8, e);
}

TEST(EnumTemplate2, DESC)
{
	EnumTest e = EnumTest::A;
	EXPECT_EQ(0, e.desc().size());

	e = EnumTest::B;
	EXPECT_STREQ(TEXT("bd"), e.desc().c_str());

	e = EnumTest::C;
	EXPECT_STREQ(TEXT(""), e.desc().c_str());

	e = EnumTest::D;
	EXPECT_STREQ(TEXT("dd"), e.desc().c_str());

	e = EnumTest::E;
	EXPECT_STREQ(TEXT(""), e.desc().c_str());
}

TEST(EnumTemplate2, STR)
{
	EnumTest e = EnumTest::A;
	EXPECT_STREQ(TEXT("A"),e.str().c_str());

	e = EnumTest::B;
	EXPECT_STREQ(TEXT("B"), e.str().c_str());

	e = EnumTest::C;
	EXPECT_STREQ(TEXT("C"), e.str().c_str());

	e = EnumTest::D;
	EXPECT_STREQ(TEXT("D"), e.str().c_str());

	e = EnumTest::E;
	EXPECT_STREQ(TEXT("E"), e.str().c_str());
}

// TEST(EnumTemplate2test, Demo)
// {
// 	EnumTest e = EnumTest::A;
// 	EXPECT_EQ(0, e);
// 	EXPECT_EQ(0, e.desc().size());
// 	EXPECT_STREQ(TEXT("A"),e.str().c_str());
// 	
// 	e = EnumTest::B;
// 	EXPECT_EQ(1, e);
// 	EXPECT_STREQ(TEXT("bd"), e.desc().c_str());
// 	EXPECT_STREQ(TEXT("B"), e.str().c_str());
// 
// 	e = EnumTest::C;
// 	EXPECT_EQ(5, e);
// 	EXPECT_STREQ(TEXT(""), e.desc().c_str());
// 	EXPECT_STREQ(TEXT("C"), e.str().c_str());
// 
// 	e = EnumTest::D;
// 	EXPECT_EQ(7, e);
// 	EXPECT_STREQ(TEXT("dd"), e.desc().c_str());
// 	EXPECT_STREQ(TEXT("D"), e.str().c_str());
// 
// 	e = EnumTest::E;
// 	EXPECT_EQ(8, e);
// 	EXPECT_STREQ(TEXT(""), e.desc().c_str());
// 	EXPECT_STREQ(TEXT("E"), e.str().c_str());
// 
// }

// TEST(EnumTemplate2test1, Demo)
// {
// 	EnumTest e = EnumTestEx::Aa;
// 	EXPECT_EQ(0, e);
// 	EXPECT_EQ(0, e.desc().size());
// 	EXPECT_STREQ(TEXT("Aa"),e.str().c_str());
// 
// 	e = EnumTestEx::Bb;
// 	EXPECT_EQ(1, e);
// 	EXPECT_STREQ(TEXT("bds"), e.desc().c_str());
// 	EXPECT_STREQ(TEXT("Bb"), e.str().c_str());
// 
// 	e = EnumTestEx::Cc;
// 	EXPECT_EQ(5, e);
// 	EXPECT_STREQ(TEXT(""), e.desc().c_str());
// 	EXPECT_STREQ(TEXT("Cc"), e.str().c_str());
// 
// 	e = EnumTestEx::Dd;
// 	EXPECT_EQ(7, e);
// 	EXPECT_STREQ(TEXT("dds"), e.desc().c_str());
// 	EXPECT_STREQ(TEXT("Dd"), e.str().c_str());
// 
// 	e = EnumTestEx::Ee;
// 	EXPECT_EQ(8, e);
// 	EXPECT_STREQ(TEXT(""), e.desc().c_str());
// 	EXPECT_STREQ(TEXT("Ee"), e.str().c_str());
// 
// }

int EnumTestMain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc,argv);
	int ret = RUN_ALL_TESTS();
	getchar();
	return ret;
}