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
#include "boost\assert.hpp"

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
	virtual void		Delete() = 0;
};

template<typename T, typename I>
struct CCloneable : public I
{
	virtual ICloneable* Clone() const { return new T(static_cast<const T&>(*this)); }
	virtual void		Delete() { delete static_cast<T*>(this); }

protected:
	CCloneable() {};
};

template<typename T>
struct cloned_ptr
{
private:
	T * px;

	void operator==( cloned_ptr const& ) const;
	void operator!=( cloned_ptr const& ) const;

	inline T* clone(T const& obj) const { return static_cast<T*>(obj.Clone()); }

public:

	typedef T element_type;
	typedef cloned_ptr<T> this_type;

	/*explicit*/ cloned_ptr( T * p = 0 ): px( p ) {} // never throws
	/*explicit*/ cloned_ptr(const T& obj) : px(clone(obj))	{}

	cloned_ptr(cloned_ptr const & other)
		: px(other.px ? clone(*other) : 0)
	{}

	~cloned_ptr() // never throws
	{
		if(px)
		{
			T* tmp = px;
			px = 0;
			tmp->Delete();
		}
	}

	cloned_ptr& operator=(cloned_ptr const & other)
	{
		BOOST_ASSERT( &other != this ); // catch self-assign errors
		reset(clone(*other));
		return *this;
	}

	cloned_ptr& operator=(T const& obj)
	{
		reset(clone(obj));
		return *this;
	}

	cloned_ptr& operator=(T* p)
	{
		reset(p);
		return *this;
	}

	void reset(T * p = 0) // never throws
	{
		BOOST_ASSERT( p == 0 || p != px ); // catch self-reset errors
		this_type(p).swap(*this);
	}

	T & operator*() const // never throws
	{
		BOOST_ASSERT( px != 0 );
		return *px;
	}

	T * operator->() const // never throws
	{
		BOOST_ASSERT( px != 0 );
		return px;
	}

	T * get() const // never throws
	{
		return px;
	}

	void swap(cloned_ptr & b) // never throws
	{
		T * tmp = b.px;
		b.px = px;
		px = tmp;
	}
};