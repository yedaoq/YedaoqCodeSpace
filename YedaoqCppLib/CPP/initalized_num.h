/********************************************************************
	Created :	2012-9-21   10:21
	FileName:	initalized_num.h
	Author:		Yedaoquan
	Purpose:	
*********************************************************************/
#ifndef _initalized_num_h__
#define _initalized_num_h__

template<typename vt, vt init_value = 0>
struct initalized_num
{
public:
	initalized_num()
		: v_(init_value)
	{}

	vt operator+(vt rhs) const { return v_ + rhs; }
	vt operator-(vt rhs) const { return v_ - rhs; }
	vt operator*(vt rhs) const { return v_ * rhs; }
	vt operator/(vt rhs) const { return v_ / rhs; }

	bool operator==(vt rhs) const { return v_ == rhs; }
	bool operator!=(vt rhs) const { return v_ != rhs; }

	initalized_num& operator+=(vt rhs) { v_ += rhs; return *this; }
	initalized_num& operator-=(vt rhs) { v_ -= rhs; return *this; }
	initalized_num& operator*=(vt rhs) { v_ *= rhs; return *this; }
	initalized_num& operator/=(vt rhs) { v_ /= rhs; return *this; }

	initalized_num& operator=(vt rhs) { v_ = rhs; return *this; }

	vt operator++() { return ++v_; }
	vt operator++(int) { return v_++; }
	vt operator--() { return --v_; }
	vt operator--(int) { return v_--; }

	vt* operator&() const  { return &v_; }
	operator vt() const { return v_; }
	vt operator->() const {return v_; }

protected:
	vt v_;
};

#endif // _initalized_num_h__