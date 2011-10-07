#pragma once

#ifndef interface
#define interface struct
#endif

#include "Enumerator.h"

namespace NSYedaoqLayout
{
	struct LayoutSize;
	struct LayoutPoint;
	struct LayoutOffset;
	struct AnchorInfo;
	struct ResizeInfo;

	enum EnumLayoutDirection
	{
		Horizon = 0,
		Vertical,
	};

	enum EnumResizeMode
	{
		Fixed = 0,
		Zoomed,
		Auto,
	};

	enum EnumAnchorMode
	{
		Front = 0,
		Center,
		Back,
	};

	interface ILayout : ICloneable
	{
		virtual ~ILayout() {}

		virtual ILayout*			Clone() const = 0;
		virtual const ResizeInfo&	GetResizeInfo(EnumLayoutDirection dir) = 0;
		virtual int					Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea) = 0;
	};

	struct ResizeInfo
	{
	public:
		EnumResizeMode	Mode;  // resize mode

		union
		{
			long		Percent;	// Zoom Percent
			long        Fixed;
		};

		ResizeInfo() {}
		ResizeInfo(EnumResizeMode mode, long val)
			: Mode(mode), Fixed(val)
		{}

		static const ResizeInfo FillInfo;
		static const ResizeInfo FixedInfo;
	};

	struct AnchorInfo
	{
		EnumAnchorMode	Mode;
		long			Padding;

		AnchorInfo() {}
		AnchorInfo(EnumAnchorMode mode, long val)
			: Mode(mode), Padding(val)
		{}

		static const AnchorInfo AnchorFront;
		static const AnchorInfo AnchorCenter;
		static const AnchorInfo AnchorBack;
	};

	struct LayoutOffset
	{
		EnumLayoutDirection Direction;
		long				Value;

		LayoutOffset() {};

		LayoutOffset(EnumLayoutDirection dir, long val)
			: Direction(dir), Value(val)
		{}

		operator LayoutSize() const;
		operator LayoutPoint() const;
	};

	struct LayoutSize
	{
		long Width;
		long Height;

		LayoutSize() {}
		LayoutSize(long w, long h)
			: Width(w), Height(h)
		{}

		LayoutSize operator+(const LayoutSize& size) const;
		LayoutSize operator+(const LayoutPoint& pt) const;

		long& LengthAt(EnumLayoutDirection dir);
		long  LengthAt(EnumLayoutDirection dir) const;

		static LayoutSize Invalid;
		static LayoutSize Zero;
	};

	struct LayoutPoint
	{
		long X;
		long Y;

		LayoutPoint() {}
		LayoutPoint(long x, long y)
			: X(x), Y(y)
		{}

		LayoutPoint operator+(const LayoutSize& size) const;
		LayoutPoint operator+(const LayoutPoint& pt) const;

		long& CoordAt(EnumLayoutDirection dir);
		long  CoordAt(EnumLayoutDirection dir) const;

		static LayoutPoint Zero;
	};
}