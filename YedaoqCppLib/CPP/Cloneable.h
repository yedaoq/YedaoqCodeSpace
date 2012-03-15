/* ___________________________________________________________________________
@ 
@ file - Cloneable.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-3-15
@ info -
@     
/* ___________________________________________________________________________*/
#pragma once

#ifndef interface
#define interface struct
#endif

//#define REFCOUNTCLONEABLE

#ifdef REFCOUNTCLONEABLE
	#define DERIVEFROMIREFOBJECT : public IRefObject
#else
	#define DERIVEFROMIREFOBJECT
#endif

#ifdef REFCOUNTCLONEABLE
	#define IMPIREFOBJECT(x) CRefObjectImpl<x>
#else
	#define IMPIREFOBJECT(x) x
#endif

interface ICloneable DERIVEFROMIREFOBJECT
{
	virtual ICloneable* Clone() const = 0;
};

template<typename T, typename I>
struct CCloneable : public I
{
	virtual ICloneable* Clone() const { return new T(static_cast<const T&>(*this)); }
};