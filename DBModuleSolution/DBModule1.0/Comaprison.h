#pragma once

#ifndef interface
#define interface struct
#endif

template<typename T>
interface IComparison
{
	virtual int operator()(const T& lhs, const T&rhs) = 0;
	virtual ~IComparison(){}
};

template<typename T>
interface IBoolComparison
{
	virtual bool operator()(const T& lhs, const T&rhs) = 0;
	virtual ~IComparison(){}
};

static enum EnumBoolComparison
{ 
	BOOLCMP_LESS, 
	BOOLCMP_GREATER, 
	BOOLCMP_EQUAL, 
	BOOLCMP_UNEQUAL, 
	BOOLCMP_LESSEQUAL, 
	BOOLCMP_GREATEREQUAL 
};

class CINTCompareResultToBool
{
public:
	typedef bool (*FBoolFromInt)(int val);

	static bool ToLessThan(int val)			{ return val < 0; }
	static bool ToGreaterThan(int val)		{ return val > 0; }
	static bool ToEqual(int val)			{ return val == 0; }
	static bool ToUnequal(int val)			{ return val != 0; }
	static bool ToLessEqual(int val)		{ return val <= 0; }
	static bool ToGreaterEqual(int val)		{ return val >= 0; }
};

template<typename T, CINTCompareResultToBool::FBoolFromInt ADAPTER>
class CBoolComparisonAdapter4Int : public IBoolComparison<T>
{
public:
	virtual bool operator()(const T& lhs, const T&rhs){ return ADAPTER(Comparison_(lhs, rhs)); }

	CBoolComparisonAdapter4Int(IComparison& cmp)
		: Comparison_(cmp)
	{}

protected:
	IComparison& Comparison_;
};

//typedef CBoolComparisonAdapter4Int<

//template<typename T>
//class CComparisonToLessThen : public IBoolComparison<T>
//{
//public:
//	CComparisonToLessThen(IComparison<T>& comparison)
//		: Comaprison_(comparison)
//	{}
//
//	CComparisonToLessThen(const CComparisonToLessThen& other)
//		: Comaprison_(other.Comaprison_)
//	{}
//
//	virtual bool operator()(const T& lhs, const T&rhs)
//	{
//		return Comaprison_(lhs, rhs) < 0;
//	}
//
//protected:
//	IComparison<T>& Comaprison_;
//
//	CComparisonToLessThen& operator =(const CComparisonToLessThan&);
//};
//
//template<typename T>
//class CComparisonToGreaterThen : public IBoolComparison<T>
//{
//public:
//	CComparisonToGreaterThen(IComparison<T>& comparison)
//		: Comaprison_(comparison)
//	{}
//
//	CComparisonToGreaterThen(const CComparisonToGreaterThen& other)
//		: Comaprison_(other.Comaprison_)
//	{}
//
//	virtual bool operator()(const T& lhs, const T&rhs)
//	{
//		return Comaprison_(lhs, rhs) > 0;
//	}
//
//protected:
//	IComparison<T>& Comaprison_;
//
//	CComparisonToGreaterThen& operator =(const CComparisonToLessThan&);
//};
//
//template<typename T>
//class CComparisonToEqual : public IBoolComparison<T>
//{
//public:
//	CComparisonToEqual(IComparison<T>& comparison)
//		: Comaprison_(comparison)
//	{}
//
//	CComparisonToEqual(const CComparisonToEqual& other)
//		: Comaprison_(other.Comaprison_)
//	{}
//
//	virtual bool operator()(const T& lhs, const T&rhs)
//	{
//		return Comaprison_(lhs, rhs) > 0;
//	}
//
//protected:
//	IComparison<T>& Comaprison_;
//
//	CComparisonToEqual& operator =(const CComparisonToLessThan&);
//};