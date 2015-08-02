#pragma once
#include "pch.h"

class GridSpace 
{
public:
	GridSpace()
	{
		this->column = 0;
		this->row = 0;
	}

	GridSpace(int column, int row)
	{
		this->column = column;
		this->row = row;
	}

	int GetColumn() { return this->column; }
	int GetRow() { return this->row; }

protected:

private:
	int column;
	int row;
};