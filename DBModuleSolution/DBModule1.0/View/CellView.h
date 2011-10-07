#pragma once

#include <mytype.h>
#include "CellContext.h"
#include "Singlton.h"

namespace NSDBModule
{
	interface ICellView
	{
		virtual ~ICellView() = 0 {};

		virtual tstring GetValue(const tchar* view, ICellContext* ctx) = 0;
		virtual tstring GetView(const tchar* val, ICellContext* ctx) = 0;
	};

	class CCellViewNone : public ICellView, public CSingleton<CCellViewNone>
	{
	public:
		virtual tstring GetValue(LPCTSTR view, ICellContext* ctx) {return tstring(view); };
		virtual tstring GetView(LPCTSTR val, ICellContext* ctx) { return tstring(val); };
	};
}