#pragma once
#include <boost\lexical_cast.hpp>

#ifndef interface
#define interface struct
#endif

template<typename Tt, typename Ts>
interface ITypeConverter
{
	virtual Tt operator()(const Ts& val) const = 0;
	virtual ~ITypeConverter(){}
};

template<typename Tt, typename Ts>
class CTypeConverter_StaticCast : public ITypeConverter<Tt, Ts>
{
public:
	virtual Tt operator()(const Ts& val) const
	{
		return static_cast<Tt>(val);
	}
};

template<typename Tt, typename Ts>
class CTypeConverter_LexicalCast : public ITypeConverter<Tt, Ts>
{
public:
	virtual Tt operator()(const Ts& val) const
	{
		return boost::lexical_cast<Tt>(val);
	}
};
