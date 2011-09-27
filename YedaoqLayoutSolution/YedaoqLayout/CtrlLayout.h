#pragma once

#include "Layout.h"

namespace NSYedaoqLayout
{
	class CCtrlLayout : public ILayout
	{
	public:
		virtual ~CCtrlLayout(void) {}

		CCtrlLayout(HWND hWnd)
			: HWnd_(hWnd)
		{}

		CCtrlLayout(HWND hWnd, ResizeInfo resizeInfoH, ResizeInfo resizeInfoV, AnchorInfo anchorInfoH, AnchorInfo anchorInfoV)
			: HWnd_(hWnd), ResizeInfoH_(resizeInfoH), ResizeInfoV_(resizeInfoV), AnchorInfoH_(anchorInfoH), AnchorInfoV_(anchorInfoV)
		{}

		virtual ILayout*			Clone()	const	{ return new CCtrlLayout(HWnd_, ResizeInfoH_, ResizeInfoV_, AnchorInfoH_, AnchorInfoV_); }

		HWND						GetHWnd()		{ return HWnd_; }

		virtual const ResizeInfo&	GetResizeInfo(EnumLayoutDirection dir);
		virtual const AnchorInfo&	GetAnchorInfo(EnumLayoutDirection dir);

		virtual int					Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea);

	protected:

		virtual int			CalLayout(long lArea, const ResizeInfo& resizeInfo, const AnchorInfo& anchorInfo, long& length, long& offset);
		virtual int			UpdateDefaultSize();

	protected:
		HWND				HWnd_;

		ResizeInfo			ResizeInfoH_;
		ResizeInfo			ResizeInfoV_;
		AnchorInfo			AnchorInfoH_;
		AnchorInfo			AnchorInfoV_;
		
	};
}

