#pragma once

#include "Layout.h"

namespace NSYedaoqLayout
{
	class CZoomBlankLayout : public ILayout
	{
	public:
		CZoomBlankLayout(int iPercentH = 100, int iPercentV = 100);

		virtual ILayout*			Clone() const { return new CZoomBlankLayout(ResizeInfoH_.Percent, ResizeInfoV_.Percent); }
		virtual const ResizeInfo&	GetResizeInfo(EnumLayoutDirection dir);
		virtual int					Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea) { return 1; };

	protected:
		ResizeInfo		ResizeInfoH_;
		ResizeInfo		ResizeInfoV_;
	};
}
