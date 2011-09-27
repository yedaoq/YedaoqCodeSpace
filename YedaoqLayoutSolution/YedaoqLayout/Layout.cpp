#include "StdAfx.h"
#include "Layout.h"

using namespace NSYedaoqLayout;

LayoutSize LayoutSize::Invalid(-1,-1);
LayoutSize LayoutSize::Zero(0,0);
LayoutPoint LayoutPoint::Zero(0,0);

const ResizeInfo ResizeInfo::FillInfo( EnumResizeMode::Zoomed, 100);
const ResizeInfo ResizeInfo::FixedInfo( EnumResizeMode::Fixed, -1);

const AnchorInfo AnchorInfo::AnchorFront(EnumAnchorMode::Front, 3);
const AnchorInfo AnchorInfo::AnchorCenter(EnumAnchorMode::Center, 3);
const AnchorInfo AnchorInfo::AnchorBack(EnumAnchorMode::Back, 3);

LayoutOffset::operator LayoutSize() const
{
	LayoutSize lsRet(0, 0);
	lsRet.LengthAt(Direction) = Value;
	return lsRet;
}

LayoutOffset::operator LayoutPoint() const
{
	LayoutPoint lpRet(0, 0);
	lpRet.CoordAt(Direction) = Value;
	return lpRet;
}

LayoutSize LayoutSize::operator+(const LayoutSize& size) const
{
	return LayoutSize(Width + size.Width, Height + size.Height);	
}

LayoutSize LayoutSize::operator+(const LayoutPoint& pt) const
{
	return LayoutSize(Width + pt.X, Height + pt.Y);	
}

long& LayoutSize::LengthAt(EnumLayoutDirection dir) 
{
	return (dir == EnumLayoutDirection::Horizon) ? Width : Height; 
}

long LayoutSize::LengthAt(EnumLayoutDirection dir) const
{
	return (dir == EnumLayoutDirection::Horizon) ? Width : Height; 
}

LayoutPoint LayoutPoint::operator+(const LayoutSize& size) const
{
	return LayoutPoint(X + size.Width, Y + size.Height); 
}

LayoutPoint LayoutPoint::operator+(const LayoutPoint& pt) const
{
	return LayoutPoint(X + pt.X, Y + pt.Y);
}

long& LayoutPoint::CoordAt(EnumLayoutDirection dir) 
{
	return (dir == EnumLayoutDirection::Horizon) ? X : Y; 
}

long LayoutPoint::CoordAt(EnumLayoutDirection dir) const
{
	return (dir == EnumLayoutDirection::Horizon) ? X : Y; 
}