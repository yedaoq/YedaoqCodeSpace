#include "StdAfx.h"
#include "ZoomBlankLayout.h"

using namespace NSYedaoqLayout;

CZoomBlankLayout::CZoomBlankLayout(int iPercentH, int iPercentV)
{
	ResizeInfoH_.Mode = EnumResizeMode::Zoomed;
	ResizeInfoH_.Percent = iPercentH;
	ResizeInfoV_.Mode = EnumResizeMode::Zoomed;
	ResizeInfoV_.Percent = iPercentV;
}

const ResizeInfo& CZoomBlankLayout::GetResizeInfo(EnumLayoutDirection dir)
{
	return (dir == EnumLayoutDirection::Horizon) ?
		ResizeInfoH_ : ResizeInfoV_;
}
