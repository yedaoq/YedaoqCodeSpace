#include "stdafx.h"
#include "Cloneable.h"
#include "iostream"
#include "vector"

using namespace std;

struct CBase : public CCloneable<CBase, ICloneable>
{
	static int BaseCount;
	static int BaseConstruct;
	
	CBase()	{ construct(); }
	CBase(const CBase& other) { construct(); }

	~CBase()
	{
		--BaseCount;
		//cout<<"CBase Object Destruct ..."<<endl;
	}

private:
	void construct()
	{
		++BaseCount; ++BaseConstruct;
		//cout<<"CBase Object Construct ..."<<endl;
	}
};

struct CDeriveA : public CCloneable<CDeriveA, CBase>
{
	static int ACount;
	static int AConstruct;
	CDeriveA(){ construct(); }
	CDeriveA(const CDeriveA&) { construct(); }

	~CDeriveA()
	{
		--ACount;
		//cout<<"CDeriveA Object Destruct ..."<<endl;
	}

private:
	void construct()
	{
		++ACount; ++AConstruct;
		//cout<<"CDeriveA Object Construct ..."<<endl;
	}
};

struct CDeriveB : public CCloneable<CDeriveB, CBase>
{
	static int BCount;
	static int BConstruct;
	CDeriveB() { construct(); }
	CDeriveB(const CDeriveB&) { construct(); }

	~CDeriveB()
	{
		--BCount;
		//cout<<"CDeriveB Object Destruct ..."<<endl;
	}

private:
	void construct()
	{
		++BCount; ++BConstruct;
		//cout<<"CDeriveB Object Construct ..."<<endl;
	}
};

void CheckLeakage()
{
	EXPECT_EQ(0, CBase::BaseCount);
	EXPECT_EQ(0, CDeriveA::ACount);
	EXPECT_EQ(0, CDeriveB::BCount);
}

void ResetCounter()
{
	CBase::BaseCount = 0;
	CDeriveA::ACount = 0;
	CDeriveB::BCount = 0;
};

void ResetConstructor()
{
	CBase::BaseConstruct = 0;
	CDeriveA::AConstruct = 0;
	CDeriveB::BConstruct = 0;
}

int CBase::BaseCount = 0;
int CDeriveA::ACount = 0;
int CDeriveB::BCount = 0;
int CBase::BaseConstruct = 0;
int CDeriveA::AConstruct = 0;
int CDeriveB::BConstruct = 0;

TEST(Cloneable, Clone)
{
	CheckLeakage();
	ResetCounter();
	ResetConstructor();

	CBase* pa = new CDeriveA;
	EXPECT_EQ(1, CBase::BaseCount);
	EXPECT_EQ(1, CDeriveA::ACount);
	EXPECT_EQ(0, CDeriveB::BCount);
	EXPECT_EQ(1, CBase::BaseConstruct);
	EXPECT_EQ(1, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);

	CBase* paa = static_cast<CBase*>(pa->Clone());
	EXPECT_EQ(2, CBase::BaseCount);
	EXPECT_EQ(2, CDeriveA::ACount);
	EXPECT_EQ(0, CDeriveB::BCount);
	EXPECT_EQ(2, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);

	CDeriveB b;
	EXPECT_EQ(3, CBase::BaseCount);
	EXPECT_EQ(2, CDeriveA::ACount);
	EXPECT_EQ(1, CDeriveB::BCount);
	EXPECT_EQ(3, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(1, CDeriveB::BConstruct);

	paa->Delete();
	EXPECT_EQ(2, CBase::BaseCount);
	EXPECT_EQ(1, CDeriveA::ACount);
	EXPECT_EQ(1, CDeriveB::BCount);
	EXPECT_EQ(3, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(1, CDeriveB::BConstruct);

	pa->Delete();
	EXPECT_EQ(1, CBase::BaseCount);
	EXPECT_EQ(0, CDeriveA::ACount);
	EXPECT_EQ(1, CDeriveB::BCount);
	EXPECT_EQ(3, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(1, CDeriveB::BConstruct);

	CBase* pb = static_cast<CBase*>(b.Clone());
	EXPECT_EQ(2, CBase::BaseCount);
	EXPECT_EQ(0, CDeriveA::ACount);
	EXPECT_EQ(2, CDeriveB::BCount);
	EXPECT_EQ(4, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(2, CDeriveB::BConstruct);

	pb->Delete();
	EXPECT_EQ(1, CBase::BaseCount);
	EXPECT_EQ(0, CDeriveA::ACount);
	EXPECT_EQ(1, CDeriveB::BCount);
	EXPECT_EQ(4, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(2, CDeriveB::BConstruct);
};

TEST(Cloneable, ClonedPtr)
{
	CheckLeakage();
	ResetCounter();
	ResetConstructor();

	cloned_ptr<CBase> pa;
	EXPECT_EQ(0, CBase::BaseCount);
	EXPECT_EQ(0, CDeriveA::ACount);
	EXPECT_EQ(0, CDeriveB::BCount);
	EXPECT_EQ(0, CBase::BaseConstruct);
	EXPECT_EQ(0, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);

	pa = new CDeriveA;
	EXPECT_EQ(1, CBase::BaseCount);
	EXPECT_EQ(1, CDeriveA::ACount);
	EXPECT_EQ(0, CDeriveB::BCount);
	EXPECT_EQ(1, CBase::BaseConstruct);
	EXPECT_EQ(1, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);

	pa = new CDeriveA;
	EXPECT_EQ(1, CBase::BaseCount);
	EXPECT_EQ(1, CDeriveA::ACount);
	EXPECT_EQ(0, CDeriveB::BCount);
	EXPECT_EQ(2, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);

	CDeriveB b;
	EXPECT_EQ(2, CBase::BaseCount);
	EXPECT_EQ(1, CDeriveA::ACount);
	EXPECT_EQ(1, CDeriveB::BCount);
	EXPECT_EQ(3, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(1, CDeriveB::BConstruct);

	pa = b;
	EXPECT_EQ(2, CBase::BaseCount);
	EXPECT_EQ(0, CDeriveA::ACount);
	EXPECT_EQ(2, CDeriveB::BCount);
	EXPECT_EQ(4, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(2, CDeriveB::BConstruct);

	pa.reset();
	EXPECT_EQ(1, CBase::BaseCount);
	EXPECT_EQ(0, CDeriveA::ACount);
	EXPECT_EQ(1, CDeriveB::BCount);
	EXPECT_EQ(4, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(2, CDeriveB::BConstruct);
}

TEST(Cloneable, Array)
{
	CheckLeakage();
	ResetCounter();
	ResetConstructor();

	std::vector<cloned_ptr<CBase> > objs;
	objs.reserve(30);

	cloned_ptr<CBase> p = CDeriveA();
	EXPECT_EQ(1, CBase::BaseCount);
	EXPECT_EQ(1, CDeriveA::ACount);
	EXPECT_EQ(0, CDeriveB::BCount);
	EXPECT_EQ(2, CBase::BaseConstruct);
	EXPECT_EQ(2, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);

	ResetConstructor();

	objs.push_back(p);  
	EXPECT_EQ(2, CBase::BaseCount);
	EXPECT_EQ(2, CDeriveA::ACount);
	EXPECT_EQ(0, CDeriveB::BCount);
	EXPECT_EQ(1, CBase::BaseConstruct);
	EXPECT_EQ(1, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);

	ResetConstructor();

	p = new CDeriveB();	
	EXPECT_EQ(2, CBase::BaseCount);
	EXPECT_EQ(1, CDeriveA::ACount);
	EXPECT_EQ(1, CDeriveB::BCount);
	EXPECT_EQ(1, CBase::BaseConstruct);
	EXPECT_EQ(0, CDeriveA::AConstruct);
	EXPECT_EQ(1, CDeriveB::BConstruct);

	ResetConstructor();

	objs.push_back(p);			
	EXPECT_EQ(3, CBase::BaseCount);
	EXPECT_EQ(1, CDeriveA::ACount);
	EXPECT_EQ(2, CDeriveB::BCount);
	EXPECT_EQ(1, CBase::BaseConstruct);
	EXPECT_EQ(0, CDeriveA::AConstruct);
	EXPECT_EQ(1, CDeriveB::BConstruct);

	ResetConstructor();

	objs.push_back(CDeriveA());	// obj ->-> cloned_ptr ->-> array member : 3
	EXPECT_EQ(4, CBase::BaseCount);
	EXPECT_EQ(2, CDeriveA::ACount);
	EXPECT_EQ(2, CDeriveB::BCount);
	EXPECT_EQ(3, CBase::BaseConstruct);
	EXPECT_EQ(3, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);

	ResetConstructor();

	objs.push_back(new CDeriveB());	// obj -> cloned_ptr ->-> array member : 2		
	EXPECT_EQ(5, CBase::BaseCount);	// there will be no memory leakage
	EXPECT_EQ(2, CDeriveA::ACount);
	EXPECT_EQ(3, CDeriveB::BCount);
	EXPECT_EQ(2, CBase::BaseConstruct);
	EXPECT_EQ(0, CDeriveA::AConstruct);
	EXPECT_EQ(2, CDeriveB::BConstruct);

	ResetConstructor();

	objs.push_back(cloned_ptr<CBase>());
	(*objs.rbegin()) = new CDeriveA();
	EXPECT_EQ(6, CBase::BaseCount);
	EXPECT_EQ(3, CDeriveA::ACount);
	EXPECT_EQ(3, CDeriveB::BCount);
	EXPECT_EQ(1, CBase::BaseConstruct);
	EXPECT_EQ(1, CDeriveA::AConstruct);
	EXPECT_EQ(0, CDeriveB::BConstruct);
}

TEST(Cloneable, ArrayAssign)
{
	CheckLeakage();
	ResetCounter();
	ResetConstructor();

	std::vector<cloned_ptr<CBase> > objs;
	objs.reserve(30);

	objs.push_back(new CDeriveA());
	objs.push_back(new CDeriveB());
	objs.push_back(new CDeriveA());
	objs.push_back(new CDeriveB());

	EXPECT_EQ(4, CBase::BaseCount);
	EXPECT_EQ(2, CDeriveA::ACount);
	EXPECT_EQ(2, CDeriveB::BCount);
	EXPECT_EQ(8, CBase::BaseConstruct);
	EXPECT_EQ(4, CDeriveA::AConstruct);
	EXPECT_EQ(4, CDeriveB::BConstruct);

	std::vector<cloned_ptr<CBase> > temp = objs;

	EXPECT_EQ(8, CBase::BaseCount);
	EXPECT_EQ(4, CDeriveA::ACount);
	EXPECT_EQ(4, CDeriveB::BCount);
	EXPECT_EQ(12, CBase::BaseConstruct);
	EXPECT_EQ(6, CDeriveA::AConstruct);
	EXPECT_EQ(6, CDeriveB::BConstruct);
}

int TestCloneMain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc,argv);
	int ret = RUN_ALL_TESTS();
	getchar();
	return ret;
}