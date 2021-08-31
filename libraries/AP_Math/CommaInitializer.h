#pragma once

#include "SimpleMatrix.h"

template <typename T>
class SimpleMatrix;

template <typename T>
class CommaInitializer
{
	
	SimpleMatrix<T>* tempMat;
	SimpleMatrix<T>& mat;

	uint8_t row;
	uint8_t col;
	uint8_t rowMax;
	uint8_t colMax;

public:

	inline CommaInitializer<T>(SimpleMatrix<T>& mat_init, const T& val)
		: mat(mat_init), row(0), col(1)
	{
		colMax = mat_init.getCols();
		rowMax = mat_init.getRows();
		tempMat = new SimpleMatrix<T>(rowMax, colMax);
		if (mat_init.getRows() > 0 && mat_init.getCols() > 0)
			(*tempMat)(0, 0) = val;
	}

	inline ~CommaInitializer<T>()
	{
		finish();
		delete tempMat;
	}

	SimpleMatrix<T>& finish()
	{
		if (row == rowMax - 1 && col == colMax)
			mat = *tempMat;
		else if (row < rowMax )
			std::cout << "Too few coefficients for comma initializer\n";
		else if (row >= rowMax)
			std::cout << "Too much coefficients for comma initializer\n";
		return mat;
	}

	CommaInitializer& operator,(const T& val)
	{
		if (col == colMax)
		{
			row++;
			col = 0;
		}
		if (row >= rowMax) // too many rows passed to commaInit
		{
			return *this;
		}
		(*tempMat)(row, col++) = val;
		return *this;
	}

};