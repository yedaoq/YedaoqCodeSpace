#pragma once

template<typename T>
class CCellFactory;

#include "GridCell.h"
#include "GridCellCheck.h"
#include "GridCellComboBox.h"
#include "GridCellLink.h"
//class CGridCellBase;
//class CGridCellComboBox;
//class CGridCellCheck;
//class CGridCellLink;
//class CGridCell;

class CCellFactoryProvider
{
public:
	static CCellFactory<CGridCellComboBox>	ComboBoxCellFactory;
	static CCellFactory<CGridCellCheck>		CheckBoxCellFactory;
	static CCellFactory<CGridCellLink>		LinkCellFactory;
	static CCellFactory<CGridCell>			DefaultCellFactory;
};