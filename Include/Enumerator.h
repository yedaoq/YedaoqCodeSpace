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

	virtual bool			MoveNext()		= 0;
	virtual bool			MoveNext(T&)	= 0;
	virtual const T&		Current()		= 0;
	virtual void			Reset()			= 0;
};

template<typename T>
class CEnumeratorBase : public IMPIREFOBJECT(IEnumerator<T>)
{
public:
	virtual ~CEnumeratorBase() = 0 {}
	virtual bool		MoveNext()		= 0;
	virtual const T&	Current()		= 0;
	virtual void		Reset()			= 0;

	virtual bool MoveNext(T& obj)
	{
		if(MoveNext())
		{
			obj = Current();
			return true;
		}
		return false;
	}
};

template<typename Tt, typename Ts, typename Fc>
class CConvertEnumeratorEx : public CEnumeratorBase<Tt>
{
public:
	~CConvertEnumeratorEx()
	{
		if(m_SourceOwned && m_Source) delete m_Source;
		m_Source = 0;
	}

	CConvertEnumeratorEx()
		: m_SourceOwned(0), m_Source(0)
	{}

	CConvertEnumeratorEx(IEnumerator<Ts>* source, Fc convert)
		: m_SourceOwned(false), m_Source(source), m_Converter(convert)
	{}

	CConvertEnumeratorEx(IEnumerator<Ts>& source, Fc convert)
		: m_SourceOwned(true), m_Source(static_cast<IEnumerator<Ts>*>(source.Clone())), m_Converter(convert)
	{}

	CConvertEnumeratorEx(const CConvertEnumeratorEx& other)
		: m_SourceOwned(other.m_SourceOwned), m_Converter(other.m_Converter)
	{
		m_Source = m_SourceOwned ? static_cast<IEnumerator<T>*>(other.m_Source->Clone()) : other.m_Source;
	}

	CConvertEnumeratorEx& operator=(const CConvertEnumeratorEx& other)
	{
		IEnumerator<Ts>* tmp = m_SourceOwned ? m_Source : 0;

		m_SourceOwned = other.m_SourceOwned;
		m_Source = m_SourceOwned ? static_cast<IEnumerator<T>*>(other.m_Source->Clone()) : other.m_Source;
		m_Converter = other.m_Converter;

		if(tmp) delete tmp;
		return *this;
	}

	virtual bool MoveNext()
	{
		return m_Source->MoveNext();
	}

	virtual const Tt& Current()
	{
		m_InterMediate = m_Converter(m_Source->Current());
		return m_InterMediate;
	}

	virtual void Reset()
	{
		m_Source->Reset();
	}

	virtual ICloneable* Clone() const
	{
		return new CConvertEnumeratorEx(*this);
	}

	bool				m_SourceOwned;
	IEnumerator<Ts>*	m_Source;	
	Fc					m_Converter;
	Tt					m_InterMediate;
};

template<typename Tt, typename Ts, typename Fc>
class CConvertEnumerator : public CEnumeratorBase<Tt>
{
public:

	~CConvertEnumerator()
	{
		if(m_SourceOwned && m_Source) delete m_Source;
		m_Source = 0;
	}

	CConvertEnumerator()
		: m_SourceOwned(0), m_Source(0)
	{}

	CConvertEnumerator(IEnumerator<Ts>* source, Fc convert)
		: m_SourceOwned(false), m_Source(source), m_Converter(convert)
	{}

	CConvertEnumerator(IEnumerator<Ts>& source, Fc convert)
		: m_SourceOwned(true), m_Source(static_cast<IEnumerator<Ts>*>(source.Clone())), m_Converter(convert)
	{}

	CConvertEnumerator(const CConvertEnumerator& other)
		: m_SourceOwned(other.m_SourceOwned), m_Converter(other.m_Converter)
	{
		m_Source = m_SourceOwned ? static_cast<IEnumerator<Ts>*>(other.m_Source->Clone()) : other.m_Source;
	}

	CConvertEnumerator& operator=(const CConvertEnumerator& other)
	{
		IEnumerator<Ts>* tmp = m_SourceOwned ? m_Source : 0;

		m_SourceOwned = other.m_SourceOwned;
		m_Source = m_SourceOwned ? static_cast<IEnumerator<T>*>(other.m_Source->Clone()) : other.m_Source;
		m_Converter = other.m_Converter;

		if(tmp) delete tmp;
		return *this;
	}

	virtual bool MoveNext()
	{
		return m_Source->MoveNext();
	}

	virtual const Tt& Current()
	{
		return m_Converter(m_Source->Current());
	}

	virtual void Reset()
	{
		m_Source->Reset();
	}

	virtual ICloneable* Clone() const
	{
		return new CConvertEnumerator(*this);
	}

	bool				m_SourceOwned;
	IEnumerator<Ts>*	m_Source;	
	Fc					m_Converter;
};

template<typename T, typename Ff>
class CFilterEnumerator : public CEnumeratorBase<T>
{
public:

	~CFilterEnumerator()
	{
		if(m_SourceOwned && m_Source)
		{
			delete m_Source;			
		}
		m_Source = 0;
	}

	CFilterEnumerator()
		: m_SourceOwned(false), m_Source(0)
	{}

	CFilterEnumerator(IEnumerator<T>* source, Ff filter)
		: m_SourceOwned(false), m_Source(source), m_Filter(filter)
	{}

	CFilterEnumerator(IEnumerator<T>& source, Ff filter)
		: m_SourceOwned(true), m_Source(static_cast<IEnumerator<T>*>(source.Clone())), m_Filter(filter)
	{}

	CFilterEnumerator(const CFilterEnumerator& other)
		: m_SourceOwned(other.m_SourceOwned), m_Filter(other.m_Filter)
	{
		m_Source = m_SourceOwned ? static_cast<IEnumerator<T>*>(other.m_Source->Clone()) : other.m_Source;
	}

	CFilterEnumerator& operator=(const CFilterEnumerator& other)
	{
		IEnumerator<T>* tmp = m_SourceOwned ? m_InnerSource : 0;

		m_SourceOwned = other.m_SourceOwned;
		m_Source = m_SourceOwned ? static_cast<IEnumerator<T>*>(other.m_Source->Clone()) : other.m_Source;
		m_Filter = other.m_Filter;

		if(tmp)	delete tmp;

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

	virtual const T& Current()
	{
		return m_Source->Current();
	}

	virtual void Reset()
	{
		m_Source->Reset();
	}

	virtual ICloneable* Clone() const
	{
		return new CFilterEnumerator(*this);
	}

	bool			m_SourceOwned;
	IEnumerator<T>* m_Source;	
	Ff				m_Filter;
};

template<typename T>
class CRangeEnumerator : public CEnumeratorBase<T>
{
public:
	CRangeEnumerator()
		: current_(last_), beforefirst_(false)
	{}

	CRangeEnumerator(const T& begin, const T& end)
		: first_(begin), last_(end), current_(begin), beforefirst_(true)
	{}

	CRangeEnumerator(const CRangeEnumerator& other)
		: first_(other.first_), last_(other.last_), current_(other.current_), beforefirst_(true)
	{}

	CRangeEnumerator& operator=(const CRangeEnumerator& other)
	{
		first_ = other.first_;
		last_ = other.last_;
		current_ = other.current_;
		beforefirst_ = other.beforefirst_;

		return *this;
	}

	virtual bool MoveNext()
	{
		if(beforefirst_)
		{
			beforefirst_ = false;
			current_ = first_;
		}
		else if(current_ != last_)
		{
			++current_;
		}

		return current_ != last_;
	}

	virtual const T& Current()
	{
		_ASSERT(!beforefirst_ && current_ != last_);
		return current_ ;
	}

	virtual void Reset()
	{
		beforefirst_ = true;
	}

	virtual ICloneable* Clone() const
	{
		return new CRangeEnumerator(*this);
	}

protected:
	T		first_;
	T		last_;
	T		current_;
	bool	beforefirst_;
};

template<typename iter_t, typename value_t = typename iter_t::value_type>
class CIteratorEnumerator : public CEnumeratorBase<value_t>
{
public:
	CIteratorEnumerator(){}

	CIteratorEnumerator(const iter_t& begin, const iter_t& end)
		: inner_(begin, end)
	{}

	virtual bool MoveNext()
	{
		return inner_.MoveNext();
	}

	virtual const value_t& Current()
	{
		return *inner_.Current();
	}

	virtual void Reset()
	{
		inner_.Reset();
	}

	virtual ICloneable* Clone() const
	{
		return new CIteratorEnumerator(*this);
	}

protected:
	CRangeEnumerator<iter_t> inner_;
};

template<typename iter_t>
CIteratorEnumerator<iter_t> make_iterator_enumerator(iter_t begin, iter_t end)
{
	return CIteratorEnumerator<iter_t>(begin, end);
}

template<typename value_t, typename iter_t>
CIteratorEnumerator<iter_t, value_t> make_iterator_enumerator_ex(iter_t begin, iter_t end)
{
	return CIteratorEnumerator<iter_t, value_t>(begin, end);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumerator<Tt, Ts, Fc> make_convert_enumerator(IEnumerator<Ts>* source, Fc converter)
{
	return CConvertEnumerator<Tt, Ts, Fc>(source, converter);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumerator<Tt, Ts, Fc> make_convert_enumerator(IEnumerator<Ts>& source, Fc converter)
{
	return CConvertEnumerator<Tt, Ts, Fc>(source, converter);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumeratorEx<Tt, Ts, Fc> make_convert_enumerator_ex(IEnumerator<Ts>* source, Fc converter)
{
	return CConvertEnumeratorEx<Tt, Ts, Fc>(source, converter);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumeratorEx<Tt, Ts, Fc> make_convert_enumerator_ex(IEnumerator<Ts>& source, Fc converter)
{
	return CConvertEnumeratorEx<Tt, Ts, Fc>(source, converter);
}

template<typename T, typename Ff>
CFilterEnumerator<T, Ff> make_filter_enumerator(IEnumerator<T>* source, Ff filter)
{
	return CFilterEnumerator<T, Ff>(source, filter);
}

template<typename T, typename Ff>
CFilterEnumerator<T, Ff> make_filter_enumerator(IEnumerator<T>& source, Ff filter)
{
	return CFilterEnumerator<T, Ff>(source, filter);
}

template<typename iter_t>
CIteratorEnumerator<iter_t>* new_iterator_enumerator(iter_t begin, iter_t end)
{
	return new CIteratorEnumerator<iter_t>(begin, end);
}

template<typename value_t, typename iter_t>
CIteratorEnumerator<iter_t, value_t>* new_iterator_enumerator_ex(iter_t begin, iter_t end)
{
	return new CIteratorEnumerator<iter_t, value_t>(begin, end);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumerator<Tt, Ts, Fc>* new_convert_enumerator(IEnumerator<Ts>* source, Fc converter)
{
	return new CConvertEnumerator<Tt, Ts, Fc>(source, converter);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumerator<Tt, Ts, Fc>* new_convert_enumerator(IEnumerator<Ts>& source, Fc converter)
{
	return new CConvertEnumerator<Tt, Ts, Fc>(source, converter);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumeratorEx<Tt, Ts, Fc>* new_convert_enumerator_ex(IEnumerator<Ts>* source, Fc converter)
{
	return new CConvertEnumeratorEx<Tt, Ts, Fc>(source, converter);
}

template<typename Tt, typename Ts, typename Fc>
CConvertEnumeratorEx<Tt, Ts, Fc>* new_convert_enumerator_ex(IEnumerator<Ts>& source, Fc converter)
{
	return new CConvertEnumeratorEx<Tt, Ts, Fc>(source, converter);
}

template<typename T, typename Ff>
CFilterEnumerator<T, Ff>* new_filter_enumerator(IEnumerator<T>* source, Ff filter)
{
	return new CFilterEnumerator<T, Ff>(source, filter);
}

template<typename T, typename Ff>
CFilterEnumerator<T, Ff>* new_filter_enumerator(IEnumerator<T>& source, Ff filter)
{
	return new CFilterEnumerator<T, Ff>(source, filter);
}

