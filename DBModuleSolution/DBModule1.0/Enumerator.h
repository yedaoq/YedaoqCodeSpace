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

template<typename T>
interface IEnumerator : public ICloneable
{
	virtual ~IEnumerator() = 0 {}

	virtual bool		MoveNext()		= 0;
	virtual bool		MoveNext(T&)	= 0;
	virtual const T&	Current()		= 0;
	virtual void		Reset()			= 0;

};

template<typename Tt, typename Ts, typename Fc>
class CConvertEnumerator : public IMPIREFOBJECT(IEnumerator<Tt>)
{
public:
	CConvertEnumerator()
		: m_Source(0)
	{}

	CConvertEnumerator(IEnumerator<Ts> *source, Fc convert)
		: m_Source(source), m_Converter(convert)
	{}

	CConvertEnumerator(const CConvertEnumerator& other)
	{
		m_Source = other.m_Source;
		m_Converter = other.m_Converter;
	}

	CConvertEnumerator& operator=(const CConvertEnumerator& other)
	{
		m_Source = other.m_Source;
		m_Converter = other.m_Converter;
		return *this;
	}

	virtual bool MoveNext()
	{
		Ts tmp;
		if(m_Source->MoveNext(tmp))
		{
			return true;
		}
		return false;
	}

	virtual bool MoveNext(Tt& obj)
	{
		if(m_Source->MoveNext())
		{
			obj = Current();
			return true;
		}
		return false;
	}

	virtual const Tt& Current()
	{
		return m_Converter(m_Source->Current());
	}

	virtual void Reset()
	{
		m_Source->Reset();
	}

	IEnumerator<Ts>*	m_Source;
	Fc					m_Converter;
};

template<typename T, typename Ff>
class CFilterEnumerator : public IMPIREFOBJECT(IEnumerator<T>)
{
public:
	CFilterEnumerator()
		:m_Source(0)
	{}

	CFilterEnumerator(IEnumerator<T> *source, Ff filter)
		: m_Source(source), m_Filter(filter)
	{}

	CFilterEnumerator(const CFilterEnumerator& other)
	{
		m_Source = other.m_Source;
		m_Filter = other.m_Filter;
	}

	CFilterEnumerator& operator=(const CFilterEnumerator& other)
	{
		m_Source = other.m_Source;
		m_Filter = other.m_Filter;
		return *this;
	}

	virtual bool MoveNext()
	{
		while(m_Source->MoveNext())
		{
			if(m_Filter(Current()))
			{
				return true;
			}
		}

		return false;
	}

	virtual bool MoveNext(T& obj)
	{
		while(m_Source->MoveNext(obj))
		{
			if(m_Filter(obj))
			{
				return true;
			}
		}

		return false;
	}

	virtual const T& Current()
	{
		return m_Source->Current();
	}

	virtual void Reset()
	{
		m_Source->Reset();
	}

	IEnumerator<T>* m_Source;
	Ff				m_Filter;
};

template<typename iter_t>
class CIteratorEnumerator : public IMPIREFOBJECT(IEnumerator<typename iter_t::value_type>)
{
public:
	CIteratorEnumerator()
		: iter_(last_), beforefirst_(false)
	{}

	CIteratorEnumerator(const iter_t& begin, const iter_t& end)
		: first_(begin), last_(end), iter_(begin), beforefirst_(true)
	{}

	CIteratorEnumerator(const CIteratorEnumerator& other)
		: first_(other.first_), last_(other.last_), iter_(other.iter_), beforefirst_(true)
	{}

	CIteratorEnumerator& operator=(const CIteratorEnumerator& other)
	{
		first_ = other.first_;
		last_ = other.last_;
		iter_ = other.iter_;
		beforefirst_ = other.beforefirst_;

		return *this;
	}

	virtual bool MoveNext()
	{
		if(beforefirst_)
		{
			beforefirst_ = false;
			iter_ = first_;
		}
		else if(iter_ != last_)
		{
			++iter_;
		}

		return iter_ != last_;
	}

	virtual bool MoveNext(typename iter_t::value_type& obj)
	{
		if(MoveNext())
		{
			obj = Current();
			return true;
		}
		return false;
	}

	virtual const typename iter_t::value_type& Current()
	{
		_ASSERT(!beforefirst_ && iter_ != last_);
		return *iter_ ;
	}

	virtual void Reset()
	{
		beforefirst_ = true;
	}

	virtual ICloneable* Clone() const
	{
		return new CIteratorEnumerator(*this);
	}

protected:
	iter_t	first_;
	iter_t	last_;
	iter_t	iter_;
	bool	beforefirst_;
};

template<typename _Container, typename _Ele>
class CIteratorEnumeratorEx : public IMPIREFOBJECT(IEnumerator<_Ele>)
{
public:
	typedef _Container<_Ele>::iterator iter_t;

	CIteratorEnumerator()
		: iter_(last_), beforefirst_(false)
	{}

	CIteratorEnumerator(const iter_t& begin, const iter_t& end)
		: first_(begin), last_(end), iter_(begin), beforefirst_(true)
	{}

	CIteratorEnumerator(const CIteratorEnumerator& other)
		: first_(other.first_), last_(other.last_), iter_(other.iter_), beforefirst_(true)
	{}

	CIteratorEnumerator& operator=(const CIteratorEnumerator& other)
	{
		first_ = other.first_;
		last_ = other.last_;
		iter_ = other.iter_;
		beforefirst_ = other.beforefirst_;

		return *this;
	}

	virtual bool MoveNext()
	{
		if(beforefirst_)
		{
			beforefirst_ = false;
			iter_ = first_;
		}
		else if(iter_ != last_)
		{
			++iter_;
		}

		return iter_ != last_;
	}

	virtual bool MoveNext(typename iter_t::value_type& obj)
	{
		if(MoveNext())
		{
			obj = Current();
			return true;
		}
		return false;
	}

	virtual const typename iter_t::value_type& Current()
	{
		_ASSERT(!beforefirst_ && iter_ != last_);
		return *iter_ ;
	}

	virtual void Reset()
	{
		beforefirst_ = true;
	}

	virtual ICloneable* Clone() const
	{
		return new CIteratorEnumerator(*this);
	}

protected:
	iter_t	first_;
	iter_t	last_;
	iter_t	iter_;
	bool	beforefirst_;
};

template<typename T>
CIteratorEnumerator<T> make_iterator_enumerator(T begin, T end)
{
	return CIteratorEnumerator<T>(begin, end);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumerator<Tt, Ts, Fc> make_convert_enumerator(IEnumerator<Ts> *source, Fc converter)
{
	return CConvertEnumerator<Tt, Ts, Fc>(source, converter);
}

template<typename T, typename Ff>
CFilterEnumerator<T, Ff> make_filter_enumerator(IEnumerator<T> *source, Ff converter)
{
	return CFilterEnumerator<T, Ff>(source, converter);
}

template<typename T>
CIteratorEnumerator<T>* new_iterator_enumerator(T begin, T end)
{
	return new CIteratorEnumerator<T>(begin, end);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumerator<Tt, Ts, Fc>* new_convert_enumerator(IEnumerator<Ts> *source, Fc converter)
{
	return new CConvertEnumerator<Tt, Ts, Fc>(source, converter);
}

template<typename T, typename Ff>
CFilterEnumerator<T, Ff>* new_filter_enumerator(IEnumerator<T> *source, Ff converter)
{
	return new CFilterEnumerator<T, Ff>(source, converter);
}

