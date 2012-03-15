#include "stdafx.h"
#include "winerror.h"
#include "Enumerator.h"
#include <iostream>

TEST(Enumerator, empty)
{
	CEmptyEnumerator<int> etor;
	EXPECT_EQ(etor.MoveNext(), false);

	int val;
	EXPECT_EQ(etor.MoveNext(val), false);
	ASSERT_ANY_THROW(etor.Current());
}

TEST(Enumerator, range)
{
	CRangeEnumerator<int> etor(3,10);
	for (int i = 3; i < 10; ++i)
	{
		EXPECT_EQ(etor.MoveNext(), true);
		EXPECT_EQ(etor.Current(), i);
	}

	EXPECT_EQ(etor.MoveNext(), false);
	ASSERT_ANY_THROW(etor.Current());
}

TEST(Enumerator, iterator)
{
	int tmp[] = {3,4,5,6,7,8,9};
	std::vector<int> ints(tmp, tmp + 7);

	{
		IEnumerator<int>* etor = new_iterator_enumerator(ints.begin(), ints.end());
		for (int i = 3; i < 10; ++i)
		{
			EXPECT_EQ(etor->MoveNext(), true);
			EXPECT_EQ(etor->Current(), i);
		}

		EXPECT_EQ(etor->MoveNext(), false);
		ASSERT_ANY_THROW(etor->Current());

		delete etor;
	}

	{
		CIteratorEnumerator<std::vector<int>::iterator> etor(ints.begin(), ints.end());
		for (int i = 3; i < 10; ++i)
		{
			EXPECT_EQ(etor.MoveNext(), true);
			EXPECT_EQ(etor.Current(), i);
		}

		EXPECT_EQ(etor.MoveNext(), false);
		ASSERT_ANY_THROW(etor.Current());
	}
}

// TEST(Enumerator, iteratorex)
// {
// 	int tmp[] = {'3','4','5','6','7','8','9'};
// 	std::vector<int> ints(tmp, tmp + 7);
// 
// 	{
// 		IEnumerator<char>* etor = new_iterator_enumerator_ex<char>(ints.begin(), ints.end());
// 		for (int i = 3; i < 10; ++i)
// 		{
// 			EXPECT_EQ(etor->MoveNext(), true);
// 			EXPECT_EQ((char)(i + '0'), etor->Current());
// 		}
// 
// 		EXPECT_EQ(etor->MoveNext(), false);
// 		ASSERT_ANY_THROW(etor->Current());
// 
// 		delete etor;
// 	}
// 
// 	{
// 		CIteratorEnumerator<std::vector<int>::iterator, char> etor(ints.begin(), ints.end());
// 		for (int i = 3; i < 10; ++i)
// 		{
// 			EXPECT_EQ(etor.MoveNext(), true);
// 			EXPECT_EQ(etor.Current(), (char)(i + '0'));
// 		}
// 
// 		EXPECT_EQ(etor.MoveNext(), false);
// 		ASSERT_ANY_THROW(etor.Current());
// 	}
// }

class IntWrapper
{
public:
	IntWrapper(int i) : I(i) {}
	bool operator==(const IntWrapper& rhs) const { return I == rhs.I; }
	int I;
};

class IntWrapper2int
{
public:
	const int& operator()(const IntWrapper& I) const { return I.I; }
};

class IntWraperGT5
{
public:
	bool operator()(const IntWrapper& I) const { return I.I > 5; }
};

TEST(Enumerator, convert)
{
	IntWrapper tmp[] = {IntWrapper(3), IntWrapper(4), IntWrapper(5), IntWrapper(6), IntWrapper(7), IntWrapper(8), IntWrapper(9), };
	
	IEnumerator<IntWrapper>* etoro = new_iterator_enumerator_ex<IntWrapper>(tmp, tmp + 7);
	IEnumerator<int>* etor = new_convert_enumerator<int>(etoro, IntWrapper2int());

	for (int i = 3; i < 10; ++i)
	{
		EXPECT_EQ(etor->MoveNext(), true);
		EXPECT_EQ(etor->Current(), i);
	}

	EXPECT_EQ(etor->MoveNext(), false);
	ASSERT_ANY_THROW(etor->Current());

	delete etoro;
	delete etor;
}

TEST(Enumerator, filter)
{
	IntWrapper tmp[] = {IntWrapper(3), IntWrapper(4), IntWrapper(5), IntWrapper(6), IntWrapper(7), IntWrapper(8), IntWrapper(9), };

	IEnumerator<IntWrapper>* etoro = new_iterator_enumerator_ex<IntWrapper, IntWrapper*>(tmp, tmp + 7);
	IEnumerator<IntWrapper>* etor = new_filter_enumerator(etoro, IntWraperGT5());

	for (int i = 6; i < 10; ++i)
	{
		EXPECT_EQ(etor->MoveNext(), true);
		EXPECT_EQ(etor->Current().I, i);
	}

	EXPECT_EQ(etor->MoveNext(), false);
	ASSERT_ANY_THROW(etor->Current());

	delete etoro;
	delete etor;
}

TEST(Enumerator, clone)
{
	IntWrapper tmp[] = {IntWrapper(3), IntWrapper(4), IntWrapper(5), IntWrapper(6), IntWrapper(7), IntWrapper(8), IntWrapper(9), };

	IEnumerator<IntWrapper>* etoro = new_iterator_enumerator_ex<IntWrapper, IntWrapper*>(tmp, tmp + 7);
	IEnumerator<IntWrapper>* etora = new_filter_enumerator(etoro, IntWraperGT5());
	IEnumerator<IntWrapper>* etorb = new_filter_enumerator(*etoro, IntWraperGT5());

	for (int i = 6; i < 10; ++i)
	{
		EXPECT_EQ(etora->MoveNext(), true);
		EXPECT_EQ(etora->Current().I, i);

		EXPECT_EQ(etorb->MoveNext(), true);
		EXPECT_EQ(etorb->Current(), etora->Current());
		EXPECT_EQ(etoro->Current(), etorb->Current());
	}

	EXPECT_EQ(etora->MoveNext(), false);
	ASSERT_ANY_THROW(etora->Current());

	delete etoro;
	delete etora;
	delete etorb;
}

int TestEnumeratorMain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc,argv);
	int ret = RUN_ALL_TESTS();
	getchar();
	return ret;
}