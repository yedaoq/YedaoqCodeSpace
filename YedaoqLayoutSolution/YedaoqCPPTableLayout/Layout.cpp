#include "Layout.h"

using namespace NSYedaoqLayout;

LayoutSize LayoutSize::Invalid = {-1,-1};
LayoutSize LayoutSize::Zero = {0,0};

LayoutPoint LayoutPoint::Zero = {0,0};

LayoutSize& LayoutSize::operator+(const LayoutSize& size)
{
	Width += size.Width; 
	Height += size.Height; 
	return *this; 
}

LayoutSize& LayoutSize::operator+(const LayoutPoint& pt)
{
	Width += pt.X; 
	Height += pt.Y; 
	return *this; 
}

LayoutSize& LayoutSize::operator+(const LayoutOffset& offset)
{
	if(offset.Direction == EnumLayoutDirection::Horizon)
	{
		Width += offset.Value;
	}
	else
	{
		Height += offset.Value;
	}

	return *this;
}

LayoutPoint& LayoutPoint::operator+(const LayoutSize& size)
{
	X += size.Width; 
	Y += size.Height; 
	return *this; 
}

LayoutPoint& LayoutPoint::operator+(const LayoutPoint& pt)
{
	X += pt.X; 
	Y += pt.Y; 
	return *this; 
}

LayoutPoint& LayoutPoint::operator+(const LayoutOffset& offset)
{
	if(offset.Direction == EnumLayoutDirection::Horizon)
	{
		X += offset.Value;
	}
	else
	{
		Y += offset.Value;
	}

	return *this;
}