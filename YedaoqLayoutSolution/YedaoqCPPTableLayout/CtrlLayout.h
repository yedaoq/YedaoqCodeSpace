#pragma once

#include "Layout.h"

namespace NSYedaoqLayout
{
	class CCtrlLayout
	{
	public:
		virtual ~CCtrlLayout(void) {}

		CCtrlLayout(HWND hWnd)
			: HWnd_(hWnd), Size_(LayoutSize::Invalid)
		{}

		CCtrlLayout(HWND hWnd, ResizeInfo resizeInfo, AnchorInfo anchorInfo)
			: HWnd_(hWnd), Size_(LayoutSize::Invalid), ResizeInfo_(resizeInfo), AnchorInfo_(anchorInfo)
		{}

		virtual ILayout*	Clone()			{ return new CCtrlLayout(HWnd_, ResizeInfo_, AnchorInfo_); }

		HWND				GetHWnd()		{ return HWnd_; }
		ResizeInfo&			GetResizeInfo() { return ResizeInfo_; }
		AnchorInfo&			GetAnchorInfo() { return AnchorInfo_; }

		virtual LayoutSize	AreaAdapt(const LayoutSize& lsContainer);
		virtual int			Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea);

	protected:
		virtual LayoutSize	SizeAdapt(const LayoutSize& lsContainer);

		virtual long		CalSize(long lContainer, long lCurrent, EnumResizeMode resizeMode, int zoomPercent, EnumAnchorMode anchorMode, int padding);
		virtual long		CalOffset(long lArea, long lCtrl, EnumAnchorMode anchorMode, int padding);

		virtual LayoutSize&	GetCtrlSize();
		virtual int			SetCtrlSize(const LayoutSize&);

	protected:
		HWND				HWnd_;
		LayoutSize			Size_;

		ResizeInfo			ResizeInfo_;
		AnchorInfo			AnchorInfo_;

		
	};
}

