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
	typedef bool (*ConvertBoolFromInt)(int val);

	static bool ToLessThan(int val)			{ return val < 0; }
	static bool ToGreaterThan(int val)		{ return val > 0; }
	static bool ToEqual(int val)			{ return val == 0; }
	static bool ToUnequal(int val)			{ return val != 0; }
	static bool ToLessEqual(int val)		{ return val <= 0; }
	static bool ToGreaterEqual(int val)		{ return val >= 0; }
};

template<typename T, CINTCompareResultToBool::ConvertBoolFromInt ADAPTER>
class CBoolComparisonAdapter4Int : public IBoolComparison<T>
{
public:
	virtual bool operator()(const T& lhs, const T&rhs){ return ADAPTER(Comparison_(lhs, rhs)); }

	CBoolComparisonAdapter4Int(IComparison& cmp)
		: Comparison_(cmp)
	{}

	CBoolComparisonAdapter4Int(const CBoolComparisonAdapter4Int& other)
		: Comparison_(other.Comparison_)
	{}

protected:
	CBoolComparisonAdapter4Int& operator=(const CBoolComparisonAdapter4Int&);

	IComparison& Comparison_;
};

template<typename T>
class CComparisonToLessThen : public IBoolComparison<T>
{
public:
	CComparisonToLessThen(IComparison<T>& comparison)
		: Comaprison_(comparison)
	{}

	CComparisonToLessThen(const CComparisonToLessThen& other)
		: Comaprison_(other.Comaprison_)
	{}

	virtual bool operator()(const T& lhs, const T&rhs)
	{
		return Comaprison_(lhs, rhs) < 0;
	}

protected:
	IComparison<T>& Comaprison_;

	CComparisonToLessThen& operator =(const CComparisonToLessThen&);
};

template<typename T>
class CComparisonToGreaterThen : public IBoolComparison<T>
{
public:
	CComparisonToGreaterThen(IComparison<T>& comparison)
		: Comaprison_(comparison)
	{}

	CComparisonToGreaterThen(const CComparisonToGreaterThen& other)
		: Comaprison_(other.Comaprison_)
	{}

	virtual bool operator()(const T& lhs, const T&rhs)
	{
		return Comaprison_(lhs, rhs) > 0;
	}

protected:
	IComparison<T>& Comaprison_;
	CComparisonToGreaterThen& operator =(const CComparisonToGreaterThen&);
};

template<typename T>
class CComparisonToEqual : public IBoolComparison<T>
{
public:
	CComparisonToEqual(IComparison<T>& comparison)
		: Comaprison_(comparison)
	{}

	CComparisonToEqual(const CComparisonToEqual& other)
		: Comaprison_(other.Comaprison_)
	{}

	virtual bool operator()(const T& lhs, const T&rhs)
	{
		return Comaprison_(lhs, rhs) > 0;
	}

protected:
	IComparison<T>& Comaprison_;
	CComparisonToEqual& operator =(const CComparisonToEqual&);
};