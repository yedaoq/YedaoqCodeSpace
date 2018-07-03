#include "StdAfx.h"
#include "FlowLayout.h"
#include "CtrlLayout.h"
#include "ZoomBlankLayout.h"

using namespace NSYedaoqLayout;

CFlowLayout::~CFlowLayout()
{
	Clear();
}

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
			//_ASSERT(FALSE);
			break;
		}

		const ResizeInfo& info = (*iter)->GetResizeInfo(GetDirection());
		switch(info.Mode)
		{
		case NSYedaoqLayout::Resize_Auto:
		case NSYedaoqLayout::Resize_Fixed:
			itemSize.LengthAt(GetDirection()) = info.Fixed;
			break;
		case NSYedaoqLayout::Resize_Zoomed:
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

	if(ResizeInfo_.Mode == NSYedaoqLayout::Resize_Auto)
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
	offset.Direction = (GetDirection() == NSYedaoqLayout::Direction_Horizon)
		? NSYedaoqLayout::Direction_Vertical : NSYedaoqLayout::Direction_Horizon;

	for(ItemCollection::iterator iter = Items_.begin(); iter != Items_.end(); ++iter)
	{
		const ResizeInfo& info = (*iter)->GetResizeInfo(offset.Direction);
		switch(info.Mode)
		{
		case NSYedaoqLayout::Resize_Auto:
		case NSYedaoqLayout::Resize_Fixed:
			if(offset.Value < info.Fixed)
			{
				offset.Value = info.Fixed;
			}
			break;
		case NSYedaoqLayout::Resize_Zoomed:
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
		case NSYedaoqLayout::Resize_Auto:
		case NSYedaoqLayout::Resize_Fixed:
			lRet += info.Fixed;
			break;
		case NSYedaoqLayout::Resize_Zoomed:
			break;
		default:
			_ASSERT(FALSE);
			break;
		}
	}

	return lRet;
}

CFlowLayout* CFlowLayout::AddFlow(EnumLayoutDirection dir, ResizeInfo resizeInfo)
{
	CFlowLayout* pItem = new CFlowLayout(dir, resizeInfo);
	Items_.push_back(pItem);
	return pItem;
}

CCtrlLayout* CFlowLayout::AddCtrl(HWND hWnd,ResizeInfo resizeInfoH, ResizeInfo resizeInfoV, AnchorInfo anchorInfoH, AnchorInfo anchorInfoV)
{
	CCtrlLayout* pItem = new CCtrlLayout(hWnd, resizeInfoH, resizeInfoV, anchorInfoH, anchorInfoV);
	Items_.push_back(pItem);
	return pItem;
}

CZoomBlankLayout* CFlowLayout::AddZoomBlank(long percentH, long percentV)
{
	CZoomBlankLayout* pItem = new CZoomBlankLayout(percentH, percentV);
	Items_.push_back(pItem);
	return pItem;
}

ILayout* CFlowLayout::AddItem(const ILayout& item)
{
	ILayout* pItem = item.Clone();
	Items_.push_back(pItem);
	return pItem;
}

void CFlowLayout::Clear()
{
	for (ItemCollection::iterator iter = Items_.begin(); iter != Items_.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	Items_.clear();
}

int	CFlowLayout::Count() const
{
	return Items_.size();
}

//ILayout* CFlowLayout::operator[](int idx) const
//{
//	return Items_[idx];
//}