#pragma once

#include "Layout.h"

namespace NSYedaoqLayout
{
	class CZoomBlankLayout : public ILayout
	{
	public:
		CZoomBlankLayout(long lPercentH = 100, long lPercentV = 100)
			: ResizeInfoH_(EnumResizeMode::Zoomed, lPercentH), ResizeInfoV_(EnumResizeMode::Zoomed, lPercentV)
		{}

		virtual ILayout*			Clone() const { return new CZoomBlankLayout(ResizeInfoH_.Percent, ResizeInfoV_.Percent); }
		virtual const ResizeInfo&	GetResizeInfo(EnumLayoutDirection dir);
		virtual int					Layout(const LayoutPoint& ptBase, const LayoutSize& lyArea) { return 1; };

	protected:
		ResizeInfo		ResizeInfoH_;
		ResizeInfo		ResizeInfoV_;
	};
}
