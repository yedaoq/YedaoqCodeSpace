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

interface ICloneable
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

enum EnumStdContainer
{
	Vector,
	List,
	Set,
};

namespace __nsanonymous
{
	/*
	 a std container instantiation need two twice type-appointment in different dimensionality. one is the element type(int, float etc.), and another is the container type(set, list, vector etc.) 
	 if you want to instantiating a template with a certain element type and a uncertain container type, this may be helpful.
	*/
	template<typename T, EnumStdContainer>
	struct stdcontainer_traits
	{};

	/*
	  the following micros, _VECTOR_, _SET_, _LIST_, all defined in the MSVC std implementation file 'vector', but not necessarily for other implementation
	*/
#ifdef _VECTOR_
	template<typename T>
	struct stdcontainer_traits<T, Vector>
	{
		typedef std::vector<T> type;
	};
#endif

#ifdef _SET_
	template<typename T>
	struct stdcontainer_traits<T, Set>
	{
		typedef std::set<T> type;
	};
#endif

#ifdef _LIST_
	template<typename T>
	struct stdcontainer_traits<T, List>
	{
		typedef std::list<T> type;
	};
#endif
}

template<typename T> struct IEnumerator;

/*
  this template extent the std container when them used for keep cloned_ptr objects
  the reason is: 
     as a result of cloned_ptr's character, when you call the push interface with a T object or a T pointer,
	 a temporary cloned_ptr<T> will be constructed, and then it be duplicateded into the container. that is,
	 the object may be copyed multi-times exceed you expectation.
*/
template<typename T, EnumStdContainer c>
class cloned_container : public __nsanonymous::stdcontainer_traits<cloned_ptr<T>, c>::type
{
	struct ClonedPtr2RawPtr
	{
		T* operator()(const cloned_ptr<T>& ptr) const
		{
			return ptr.get();
		}
	};

public:
	void push_back(T* obj)
	{
		__super::push_back(cloned_ptr<T>());
		*rbegin() = obj;
	}

	void push_back(T const& obj)
	{
		__super::push_back(cloned_ptr<T>());
		*rbegin() = obj;
	}

	void push_front(T* obj)
	{
		__super::push_front(cloned_ptr<T>());
		*begin() = obj;
	}

	void push_front(T const& obj)
	{
		__super::push_front(cloned_ptr<T>());
		*begin() = obj;
	}

	IEnumerator<T*>* Enum() const
	{
		return new_convert_enumerator<T*>(
			make_iterator_enumerator(begin(), end()),
			ClonedPtr2RawPtr()
		);
	}
};