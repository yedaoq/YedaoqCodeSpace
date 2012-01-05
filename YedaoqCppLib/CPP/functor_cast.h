#pragma once

#ifndef interface
#define interface struct
#endif

template<typename Tt, typename Ts>
interface ICastFunctor
{
	virtual Tt operator()(Ts val) const = 0;
	virtual ~ICastFunctor(){}
};

template<typename Tt, typename Ts>
class CCastFunctor_Static : public ICastFunctor<Tt, Ts>
{
public:
	virtual Tt operator()(Ts val) const
	{
		return static_cast<Tt>(val);
	}
};

#include <boost\lexical_cast.hpp>
template<typename Tt, typename Ts>
class CCastFunctor_Lexical : public ICastFunctor<Tt, Ts>
{
public:
	virtual Tt operator()(Ts val) const
	{
		return boost::lexical_cast<Tt>(val);
	}
};

template<typename Tt, typename Ts>
class CCastFunctor_Dynamic : public ICastFunctor<Tt, Ts>
{
public:
	virtual Tt operator()(Ts val) const
	{
		return dynamic_cast<Tt>(val);
	}
};

template<typename Ts>
class CCastFunctor_AddressOf : public ICastFunctor<Ts*, Ts>
{
public:
	virtual Ts* operator()(Ts val) const
	{
		return &val;
	}
};

template<typename Tt, typename Ts>
class CCastFunctor_Dereference : public ICastFunctor<Tt, Ts>
{
public:
	virtual Tt operator()(Ts val) const
	{
		return *val;
	}
};