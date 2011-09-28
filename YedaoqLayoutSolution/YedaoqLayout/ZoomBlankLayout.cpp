#include "StdAfx.h"
#include "ZoomBlankLayout.h"

using namespace NSYedaoqLayout;

const ResizeInfo& CZoomBlankLayout::GetResizeInfo(EnumLayoutDirection dir)
{
	return (dir == EnumLayoutDirection::Horizon) ?
		ResizeInfoH_ : ResizeInfoV_;
}
