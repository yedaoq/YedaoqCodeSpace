#pragma once

#ifndef interface
#define interface struct
#endif

template<typename T>
interface IObjectFactory
{
	virtual ~IObjectFactory() {}
	virtual T* Create() const;
};

template<typename Base, typename Derived/*, typename Allocater*/>
class CObjectFactory : public IObjectFactory<Base>
{
	virtual Base* Create() { return new Derived; }
};