#include "CtrlLayout.h"

using namespace NSYedaoqLayout;

LayoutSize CCtrlLayout::AreaAdapt(const LayoutSize& lsContainer)
{
	LayoutSize lsRet = SizeAdapt(lsContainer);

	lsRet.Height += AnchorInfo().PaddingH * 2;
	lsRet.Width += AnchorInfo().PaddingV * 2;

	return lsRet;
}

virtual LayoutSize CCtrlLayout::SizeAdapt(const LayoutSize& lsContainer)
{
	LayoutSize lsRet = GetCtrlSize();

	lsRet.Height = CalSize(lsContainer.Height, lsRet.Height, 
		ResizeInfo().ModeHeight, ResizeInfo().ZoomHeight, AnchorInfo().AnchorModeH, AnchorInfo().PaddingH);

	lsRet.Width = CalSize(lsContainer.Width, lsRet.Width,
		ResizeInfo().ModeWidth, ResizeInfo().ZoomWidth, AnchorInfo().AnchorModeV, AnchorInfo().PaddingV);

	return lsRet;
}

long CCtrlLayout::CalSize(long lContainer, long lCurrent, EnumResizeMode resizeMode, int zoomPercent, EnumAnchorMode anchorMode, int padding)
{
	long lRet = lCurrent;

	if(lContainer < 0)
	{
		ASSERT(FALSE);
		return lRet;
	}	

	switch(resizeMode)
	{
	case EnumResizeMode::Zoomed:
		lRet = lContainer * zoomPercent / 100;
		switch(anchorMode)
		{
		case EnumAnchorMode::Front:
		case EnumAnchorMode::Back:
		case EnumAnchorMode::Center:
			lRet -= padding * 2;
			break;
		default:
			ASSERT(FALSE);
		}
		break;
	case EnumResizeMode::Fixed:
	case EnumResizeMode::Auto:
		break;
	default:
		ASSERT(FALSE);
	}

	if(lRet < 0) lRet = 0;

	return lRet;
}

long CCtrlLayout::CalOffset(long lArea, long lCtrl, EnumAnchorMode anchorMode, int padding)
{
	long lRet = 0;

	switch(anchorMode)
	{
	case EnumAnchorMode::Front:
		lRet = padding;
		break;
	case EnumAnchorMode::Back:
		lRet = lArea - lCtrl + padding;
		break;
	case EnumAnchorMode::Center:
		lRet = (lArea - lCtrl) / 2 + padding;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return lRet;
}

int	CCtrlLayout::Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea)
{
	SetCtrlSize(SizeAdapt(lyArea));
	LayoutPoint layoutPoint = ptBase;

	layoutPoint.X += CalOffset(lyArea.Height, Size_.Height, AnchorInfo().AnchorModeH, AnchorInfo().PaddingH);
	layoutPoint.Y += CalOffset(lyArea.Width, Size_.Width, AnchorInfo().AnchorModeV, AnchorInfo().PaddingV);

	::MoveWindow(GetHWnd(), layoutPoint.X, layoutPoint.Y, Size_.Width, Size_.Height, true);
}

LayoutSize& CCtrlLayout::GetCtrlSize()
{
	if(Size_ == LayoutSize::Invalid)
	{
		if(GetHWnd() == NULL || GetHWnd() == INVALID_HANDLE_VALUE)
		{
			ASSERT(FALSE);
		}
		else
		{
			RECT rect;
			::GetWindowRect(GetHWnd(), &rect);
			Size_.Height = rect.bottom;
			Size_.Width = rect.right;
		}
	}

	return Size_;
}

int	CCtrlLayout::SetCtrlSize(const LayoutSize& size)
{
	Size_ = size;
}