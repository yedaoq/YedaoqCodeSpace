#pragma once

#ifndef interface
#define interface struct
#endif

#include "Enumerator.h"

namespace NSYedaoqLayout
{
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

	struct ResizeInfo
	{
	public:
		EnumResizeMode	ModeWidth;  // resize mode of Width
		int				ZoomWidth;	// Zoom Percent of Width

		EnumResizeMode	ModeHeight;	// resize mode of Height
		int				ZoomHeight;	// resize mode of Height
	};

	enum EnumAnchorMode
	{
		Front = 0,
		Center,
		Back,
	};

	struct AnchorInfo
	{
		EnumAnchorMode	AnchorModeH;
		long			PaddingH;

		EnumAnchorMode	AnchorModeV;
		long			PaddingV;
	};

	struct LayoutOffset
	{
		EnumLayoutDirection Direction;
		long				Value;
	};

	struct LayoutSize
	{
		long Width;
		long Height;

		LayoutSize& operator+(const LayoutSize& size);
		LayoutSize& operator+(const LayoutPoint& pt);
		LayoutSize& operator+(const LayoutOffset& offset);

		static LayoutSize Invalid;
		static LayoutSize Zero;
	};

	struct LayoutPoint
	{
		long X;
		long Y;

		LayoutPoint& operator+(const LayoutSize& size);
		LayoutPoint& operator+(const LayoutPoint& pt);
		LayoutPoint& operator+(const LayoutOffset& offset);

		static LayoutPoint Zero;
	};

	interface ILayout : ICloneable
	{
		virtual ~ILayout() {}

		virtual LayoutSize	AreaAdapt(const LayoutSize& lsContainer) = 0;
		virtual int			Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea) = 0;
	};
}