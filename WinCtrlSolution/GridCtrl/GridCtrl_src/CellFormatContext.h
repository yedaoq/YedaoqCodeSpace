#pragma once
#include "mytype.h"
#include <ITextFormater.h>

struct CellFormatContext : IContext
{
	int Row;
	int Col;

	CellFormatContext(int row, int col)
		: Row(row), Col(col)
	{}
};
