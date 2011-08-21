#include "StdAfx.h"
#include <set>

class cmpA
{
public:
	cmpA(int tmp){}
	bool operator()(const int& lhs, const int& rhs) { return lhs < rhs; }

protected:
	cmpA& operator=(const cmpA& other);
	cmpA(const cmpA& other);
};

class cmpB
{
public:
	cmpB() {}
	cmpB(int tmp){}
	

	bool operator()(const int& lhs, const int& rhs) { return lhs < rhs; }
	
protected:
	cmpB& operator=(const cmpB& other);
	cmpB(const cmpB& other);
};

int mainContainComparison()
{
	{
// 		typedef cmpA cmp;
// 		std::set<int,cmp> a;
// 		a.insert(1);
// 		std::set<int,cmp> b = a;
	}

	{
// 		typedef cmpB cmp;
// 		std::set<int,cmp> a;
// 		a.insert(1);
// 		std::set<int,cmp> b = a;
	}
	
	{
// 		typedef cmpA& cmp;
// 		std::set<int,cmp> a;
// 		a.insert(1);
// 		std::set<int,cmp> b = a;
	}

	{
// 		typedef cmpB& cmp;
// 		std::set<int,cmp> a;
// 		a.insert(1);
// 		std::set<int,cmp> b = a;
	}
	
	return 1;
}