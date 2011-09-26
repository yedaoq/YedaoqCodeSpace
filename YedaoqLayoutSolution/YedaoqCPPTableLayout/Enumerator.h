#pragma once

#include "..\include\IRefObject.h"
#include "..\include\RefObjectImpl.h"

interface ICloneable : public IRefObject
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
class CConvertEnumerator : public CRefObjectImpl<IEnumerator<Tt>>
{
public:
	CConvertEnumerator()
		: m_Source(0)
	{}

	CConvertEnumerator(IEnumerator<Ts> *source, Fc convert)
		: m_Source(source), m_Converter(convert)
	{}

	CConvertEnumerator(const CConvertEnumerator& other)
		: m_Source(other.m_Source), m_Converter(other.m_Converter)
	{}

	CConvertEnumerator& operator=(const CConvertEnumerator& other)
	{
		m_Source = other.m_Source;
		m_Converter = other.m_Converter;
		return *this;
	}

	virtual bool MoveNext()
	{
		return m_Source->MoveNext();
	}

	virtual bool MoveNext(Tt& obj)
	{
		if(MoveNext())
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

	virtual ICloneable* Clone() const
	{
		return new CConvertEnumerator(*this);
	}

	IEnumerator<Ts>*	m_Source;
	Fc					m_Converter;
};

template<typename T, typename Ff>
class CFilterEnumerator : public CRefObjectImpl<IEnumerator<T>>
{
public:
	CFilterEnumerator()
		:m_Source(0)
	{}

	CFilterEnumerator(IEnumerator<T> *source, Ff filter)
		: m_Source(source), m_Filter(filter)
	{}

	CFilterEnumerator(const CFilterEnumerator& other)
		: m_Source(other.m_Source), m_Filter(other.m_Filter)
	{}

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
			if(m_Filter(m_Source->Current()))
			{
				return true;
			}
		}

		return false;
	}

	virtual bool MoveNext(T& obj)
	{
		if(MoveNext())
		{
			obj = Current();
			return true;
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

	IEnumerator<T>* m_Source;
	Ff				m_Filter;
};

template<typename iter_t>
class CIteratorEnumerator : public CRefObjectImpl<IEnumerator<typename iter_t::value_type>>
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

template<typename iter_t>
CIteratorEnumerator<iter_t> make_iterator_enumerator(const iter_t& first, const iter_t& last)
{
	return CIteratorEnumerator<iter_t>(first, last);
}

template<typename T, typename Ff>
CFilterEnumerator<T, Ff> make_filter_enumerator(IEnumerator<T> *source, Ff converter)
{
	return CFilterEnumerator<T, Ff>(source, converter);
}

template<typename iter_t>
CIteratorEnumerator<iter_t>* new_iterator_enumerator(const iter_t& first, const iter_t& last)
{
	return new CIteratorEnumerator<iter_t>(first, last);
}

template<typename T, typename Ff>
CFilterEnumerator<T, Ff>* new_filter_enumerator(IEnumerator<T> *source, Ff converter)
{
	return new CFilterEnumerator<T, Ff>(source, converter);
}