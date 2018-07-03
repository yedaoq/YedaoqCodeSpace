#include "StdAfx.h"
#include "CtrlLayout.h"

using namespace NSYedaoqLayout;

const ResizeInfo& CCtrlLayout::GetResizeInfo(EnumLayoutDirection dir)
{
	UpdateDefaultSize();

	return (dir == NSYedaoqLayout::Direction_Horizon) ?
		ResizeInfoH_ : ResizeInfoV_;
}

const AnchorInfo& CCtrlLayout::GetAnchorInfo(EnumLayoutDirection dir)
{
	return (dir == NSYedaoqLayout::Direction_Horizon) ?
		AnchorInfoH_ : AnchorInfoV_;
}

int	CCtrlLayout::UpdateDefaultSize()
{
	if((ResizeInfoH_.Mode != NSYedaoqLayout::Resize_Zoomed && ResizeInfoH_.Fixed == -1)
		|| (ResizeInfoV_.Mode != NSYedaoqLayout::Resize_Zoomed && ResizeInfoV_.Fixed == -1))
	{
		RECT rect;
		::GetWindowRect(GetHWnd(), &rect);

		if(ResizeInfoH_.Mode != NSYedaoqLayout::Resize_Zoomed)
		{
			ResizeInfoH_.Fixed = rect.right - rect.left + 2 * AnchorInfoH_.Padding;
		}

		if(ResizeInfoV_.Mode != NSYedaoqLayout::Resize_Zoomed)
		{
			ResizeInfoV_.Fixed = rect.bottom - rect.top + 2 * AnchorInfoV_.Padding;
		}
	}

	return 1;
}

int	CCtrlLayout::Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea)
{
	LayoutSize	ctrlSize = LayoutSize::Zero;
	LayoutPoint ctrlOffset = LayoutPoint::Zero;
	
	CalLayout(lyArea.Width, GetResizeInfo(NSYedaoqLayout::Direction_Horizon), GetAnchorInfo(NSYedaoqLayout::Direction_Horizon),
		ctrlSize.Width, ctrlOffset.X);

	CalLayout(lyArea.Height, GetResizeInfo(NSYedaoqLayout::Direction_Vertical), GetAnchorInfo(NSYedaoqLayout::Direction_Vertical),
		ctrlSize.Height, ctrlOffset.Y);

	::MoveWindow(GetHWnd(), ptBase.X + ctrlOffset.X, ptBase.Y + ctrlOffset.Y, ctrlSize.Width, ctrlSize.Height, false);
	return 1;
}

int CCtrlLayout::CalLayout(long lArea, const ResizeInfo& resizeInfo, const AnchorInfo& anchorInfo, long& length, long& offset)
{
	switch(resizeInfo.Mode)
	{
	case NSYedaoqLayout::Resize_Auto:
	case NSYedaoqLayout::Resize_Fixed:
		length = max(0, resizeInfo.Fixed - anchorInfo.Padding * 2);
		switch(anchorInfo.Mode)
		{
		case NSYedaoqLayout::Anchor_Front:
			offset = anchorInfo.Padding;
			break;
		case NSYedaoqLayout::Anchor_Center:
			offset = (lArea - length) / 2;
			break;
		case NSYedaoqLayout::Anchor_Back:
			offset = lArea - length - anchorInfo.Padding;
			break;
		default:
			_ASSERT(FALSE);
			break;
		}
		break;
	case NSYedaoqLayout::Resize_Zoomed:
		length = max(0, (lArea - anchorInfo.Padding * 2));
		offset = anchorInfo.Padding;
		break;
	default:
		_ASSERT(FALSE);
		break;
	}

	return 1;
}