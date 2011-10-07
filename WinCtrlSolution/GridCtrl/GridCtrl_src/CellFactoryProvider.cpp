#include "stdafx.h"
#include "CellFactoryProvider.h"

#include "CellFactory.h"

#include "GridCellComboBox.h"
#include "GridCellLink.h"
#include "GridCellCheck.h"

CCellFactory<CGridCellComboBox>	CCellFactoryProvider::ComboBoxCellFactory;
CCellFactory<CGridCellCheck>	CCellFactoryProvider::CheckBoxCellFactory;
CCellFactory<CGridCellLink>		CCellFactoryProvider::LinkCellFactory;
CCellFactory<CGridCell>			CCellFactoryProvider::DefaultCellFactory;
