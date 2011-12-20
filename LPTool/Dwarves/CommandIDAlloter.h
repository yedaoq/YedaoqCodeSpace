#pragma once

#include "Resource.h"

enum EnumCommandIDRange : WORD
{
	ToolBarCmbSearchID		= 0xF800,		// 工具栏搜索框的ID
	ToolBarBtnFindNextID	= 0xF801,		// 工具栏按钮“查找下一个”的ID

	MinViewOpID				= 0xF830,		// 分配视图所提供的“操作”的ID区间，用于创建每个视图上的私有操作按钮。
	MaxViewOpID				= 0xFAFF,		
	
	MinMenuViewID			= 0xFB00,		// 分配给菜单栏“视图”下拉列表中动态创建的菜单项（指代表）的ID区间。
	MaxMenuViewID			= 0xFFFF,
};
