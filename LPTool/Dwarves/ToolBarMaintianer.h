#pragma once

#include "Singlton.h"
class CMFCBaseToolBar;

class CToolBarMaintianer : public CSingleton<CToolBarMaintianer>
{
public:
	CToolBarMaintianer(void);
	~CToolBarMaintianer(void);

	int SetToolBar(CMFCToolBar* bar);

	int Clean();
	int InitBasicItems();

	CMFCToolBarComboBoxButton	m_ToolCmbSearch;
	CMFCToolBarButton			m_ToolBtnFindNext;

protected:
	CMFCToolBar* m_Bar;
};
