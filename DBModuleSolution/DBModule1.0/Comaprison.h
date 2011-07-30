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

	CComparisonToLessThen& operator =(const CComparisonToLessThan&);
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

	CComparisonToGreaterThen& operator =(const CComparisonToLessThan&);
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

	CComparisonToEqual& operator =(const CComparisonToLessThan&);
};