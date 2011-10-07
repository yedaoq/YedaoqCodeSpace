// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件



// TODO: 在此处引用程序需要的其他头文件
#include "GridCtrl_src\GridCellBase.h"
#include "GridCtrl_src\GridCell.h"
#include "GridCtrl_src\CellFactory.h"
#include "GridCtrl_src\GridCellComboBox.h"
#include "GridCtrl_src\GridCellCheck.h"
#include "GridCtrl_src\GridCellLink.h"
#include "GridCtrl_src\CellFactoryProvider.h"
#include "GridCtrl_src\CellTemplate.h"