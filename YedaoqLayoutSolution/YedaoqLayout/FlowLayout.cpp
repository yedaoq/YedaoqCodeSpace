#include "StdAfx.h"
#include "FlowLayout.h"
#include "CtrlLayout.h"

using namespace NSYedaoqLayout;

int	CFlowLayout::Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea)
{
	LayoutOffset itemOffset(GetDirection(), 0);
	LayoutOffset itemLength(GetDirection(), 0);
	LayoutSize  itemSize	= lyArea;

	long lAreaLength = lyArea.LengthAt(GetDirection());
	long lDynamicLength = max(lAreaLength - GetFixedItemsLength(), 0);

	for (ItemCollection::iterator iter = Items_.begin(); iter != Items_.end(); ++iter)
	{
		if(itemOffset.Value >= lAreaLength)
		{
			_ASSERT(FALSE);
			break;
		}

		const ResizeInfo& info = (*iter)->GetResizeInfo(GetDirection());
		switch(info.Mode)
		{
		case EnumResizeMode::Auto:
		case EnumResizeMode::Fixed:
			itemSize.LengthAt(GetDirection()) = info.Fixed;
			break;
		case EnumResizeMode::Zoomed:
			itemSize.LengthAt(GetDirection()) = info.Percent * lDynamicLength / 100;
			break;
		default:
			_ASSERT(FALSE);
			itemSize.LengthAt(GetDirection()) = 0;
			break;
		}

		(*iter)->Layout(ptBase + (LayoutPoint)itemOffset, itemSize);
		itemOffset.Value += itemSize.LengthAt(GetDirection());
	}
	
	return 1;
}

const ResizeInfo& CFlowLayout::GetResizeInfo(EnumLayoutDirection dir)
{
	static ResizeInfo riFill = ResizeInfo::FillInfo;

	if(ResizeInfo_.Mode == EnumResizeMode::Auto)
	{
		UpdateAutoSize();
	}

	if(dir == Direction_)
	{
		return riFill;
	}
	else
	{
		return ResizeInfo_;
	}
}

int	CFlowLayout::UpdateAutoSize()
{
	LayoutOffset offset;
	offset.Value = 0;
	offset.Direction = (GetDirection() == EnumLayoutDirection::Horizon)
		? EnumLayoutDirection::Vertical : EnumLayoutDirection::Horizon;

	for(ItemCollection::iterator iter = Items_.begin(); iter != Items_.end(); ++iter)
	{
		const ResizeInfo& info = (*iter)->GetResizeInfo(offset.Direction);
		switch(info.Mode)
		{
		case EnumResizeMode::Auto:
		case EnumResizeMode::Fixed:
			if(offset.Value < info.Fixed)
			{
				offset.Value = info.Fixed;
			}
			break;
		case EnumResizeMode::Zoomed:
			break;
		default:
			_ASSERT(FALSE);
			break;
		}
	}

	ResizeInfo_.Fixed = offset.Value;
	return 1;
}

long CFlowLayout::GetFixedItemsLength()
{
	long lRet = 0;
	for (ItemCollection::iterator iter = Items_.begin(); iter != Items_.end(); ++iter)
	{
		const ResizeInfo& info = (*iter)->GetResizeInfo(GetDirection());
		switch(info.Mode)
		{
		case EnumResizeMode::Auto:
		case EnumResizeMode::Fixed:
			lRet += info.Fixed;
			break;
		case EnumResizeMode::Zoomed:
			break;
		default:
			_ASSERT(FALSE);
			break;
		}
	}

	return lRet;
}

ILayout* CFlowLayout::AddItem(HWND hWnd,ResizeInfo resizeInfoH, ResizeInfo resizeInfoV, AnchorInfo anchorInfoH, AnchorInfo anchorInfoV)
{
	ILayout* pItem = new CCtrlLayout(hWnd, resizeInfoH, resizeInfoV, anchorInfoH, anchorInfoV);
	Items_.push_back(pItem);
	return pItem;
}

ILayout* CFlowLayout::AddItem(const ILayout& item)
{
	ILayout* pItem = item.Clone();
	Items_.push_back(pItem);
	return pItem;
}