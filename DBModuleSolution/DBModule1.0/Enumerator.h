#pragma once

#ifndef interface
#define interface struct
#endif

template<typename T>
interface IEnumerator
{
	virtual ~IEnumerator() = 0 {}

	virtual bool		MoveNext()		= 0;
	virtual bool		MoveNext(T&)	= 0;
	virtual const T&	Current()		= 0;
	virtual void		Reset()			= 0;
	
};

template<typename Tt, typename Ts, typename Fc>
class CConvertEnumerator : public IEnumerator<Tt>
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
class CFilterEnumerator : public IEnumerator<T>
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
class CIteratorEnumerator : public IEnumerator<typename iter_t::value_type>
{
public:
	CIteratorEnumerator(){}

	CIteratorEnumerator(const iter_t& begin, const iter_t& end)
		: first_(begin), last_(end), iter_(begin)
	{}

	CIteratorEnumerator(const CIteratorEnumerator& other)
		: first_(other.first_), last_(other.last_), iter_(other.iter_) 
	{}

	CIteratorEnumerator& operator=(const CIteratorEnumerator& other)
	{
		first_ = other.first_;
		last_ = other.last_;
		iter_ = other.iter_;

		return *this;
	}

	virtual bool MoveNext()
	{
		iter_t tmp = iter_;

		if(++tmp != last_)
		{
			iter_ = tmp;
			return true;
		}
		return false;
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
		_ASSERT(iter_ != last_)
		return *iter_ ;
	}

	virtual void Reset()
	{
		iter_ = first_;
	}

protected:
	iter_t first_;
	iter_t last_;
	iter_t iter_;
};

template<typename _Container, typename _Ele>
class CIteratorEnumeratorEx : public IEnumerator<_Ele>
{
public:
	typedef _Container<_Ele>::iterator iterator;

	CIteratorEnumeratorEx(){}

	CIteratorEnumeratorEx(const iterator& begin, const iterator& end)
		: first_(begin), last_(end), iter_(begin)
	{}

	CIteratorEnumeratorEx(const CIteratorEnumeratorEx& other)
		: first_(other.first_), last_(other.last_), iter_(other.iter_) 
	{}

	CIteratorEnumeratorEx& operator=(const CIteratorEnumeratorEx& other)
	{
		first_ = other.first_;
		last_ = other.last_;
		iter_ = other.iter_;

		return *this;
	}

	virtual bool MoveNext()
	{
		iterator tmp = iter_;

		if(++tmp != last_)
		{
			iter_ = tmp;
			return true;
		}
		return false;
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
		_ASSERT(iter_ != last_)
			return *iter_ ;
	}

	virtual void Reset()
	{
		iter_ = first_;
	}

protected:
	iterator first_;
	iterator last_;
	iterator iter_;
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

