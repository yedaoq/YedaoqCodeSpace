#pragma once

class CGridCellBase;

struct ICellFactory
{
	virtual CGridCellBase* CreateCell() const = 0;
};

template<typename T>
class CCellFactory : public ICellFactory
{
public:
	virtual CGridCellBase* CreateCell() const
	{
		return new T();
	}
};