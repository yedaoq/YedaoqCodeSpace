#pragma once

#ifndef interface
#define interface struct
#endif

template<typename T>
interface IComparison
{
	virtual int operator()(const T& lhs, const T&rhs) const = 0;
	virtual ~IComparison(){}
};

template<typename T>
interface IBoolComparison
{
	virtual bool operator()(const T& lhs, const T&rhs) const = 0;
	virtual ~IBoolComparison(){}
};

class CINTCompareResultToBool
{
public:
	typedef bool (*ConvertBoolFromInt)(int val);

	static bool ToLessThan(int val)			{ return val < 0; }
	static bool ToGreaterThan(int val)		{ return val > 0; }
	static bool ToEqual(int val)			{ return val == 0; }
	static bool ToUnequal(int val)			{ return val != 0; }
	static bool ToLessEqual(int val)		{ return val <= 0; }
	static bool ToGreaterEqual(int val)		{ return val >= 0; }
};

// static enum EnumBoolComparison : unsigned int
// { 
// 	BOOLCMP_LESS = (unsigned int)&CINTCompareResultToBool::ToLessEqual, 
// 	BOOLCMP_GREATER = (unsigned int)&CINTCompareResultToBool::ToGreaterThan, 
// 	BOOLCMP_EQUAL = (unsigned int)&CINTCompareResultToBool::ToEqual, 
// 	BOOLCMP_UNEQUAL = (unsigned int)&CINTCompareResultToBool::ToUnequal, 
// 	BOOLCMP_LESSEQUAL = (unsigned int)&CINTCompareResultToBool::ToLessEqual, 
// 	BOOLCMP_GREATEREQUAL = (unsigned int)&CINTCompareResultToBool::ToGreaterEqual, 
// };

// template<typename T, EnumBoolComparison ADAPTER>
// class CBoolComparisonAdapter4Int : public IBoolComparison<T>
// {
// public:
// 	virtual bool operator()(const T& lhs, const T&rhs) const 
// 	{ 
// 		return static_cast<CINTCompareResultToBool::ConvertBoolFromInt>(ADAPTER)(Comparison_(lhs, rhs)); 
// 	}
// 
// 	CBoolComparisonAdapter4Int(IComparison& cmp)
// 		: Comparison_(cmp)
// 	{}
// 
// 	CBoolComparisonAdapter4Int(const CBoolComparisonAdapter4Int& other)
// 		: Comparison_(other.Comparison_)
// 	{}
// 
// protected:
// 	CBoolComparisonAdapter4Int& operator=(const CBoolComparisonAdapter4Int&);
// 
// 	IComparison& Comparison_;
// };

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
//	virtual bool operator()(const T& lhs, const T&rhs) const
//	{
//		return Comaprison_(lhs, rhs) < 0;
//	}
//
//protected:
//	IComparison<T>& Comaprison_;
//
//	CComparisonToLessThen& operator =(const CComparisonToLessThen&);
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
//	virtual bool operator()(const T& lhs, const T&rhs) const
//	{
//		return Comaprison_(lhs, rhs) > 0;
//	}
//
//protected:
//	IComparison<T>& Comaprison_;
//	CComparisonToGreaterThen& operator =(const CComparisonToGreaterThen&);
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
//	virtual bool operator()(const T& lhs, const T&rhs) const
//	{
//		return Comaprison_(lhs, rhs) > 0;
//	}
//
//protected:
//	IComparison<T>& Comaprison_;
//	CComparisonToEqual& operator =(const CComparisonToEqual&);
//};