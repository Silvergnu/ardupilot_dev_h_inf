/*#pragma once

#include <vector>
#include <iostream>
#include "math.h"
#include <stdint.h>
#include "CommaInitializer.h"

template <typename T>
class SimpleMatrix
{

private:
    std::vector<std::vector<T> > v; // vector of vector to represent matrix
    uint8_t nRows, nCols;
public:
    // constructor from zeros
    SimpleMatrix<T>(uint8_t rows, uint8_t cols)
        : v(rows), nRows(rows), nCols(cols)
    {
        if (rows == 0 || cols == 0) // invalid matrix size
            throw std::runtime_error("Invalid matrix size");
        for (int i = 0; i < rows; i++)
            v[i].resize(cols);
    }

    //constructor to fill with values val
    SimpleMatrix<T>(uint8_t rows, uint8_t cols, T val)
        : v(std::vector<std::vector<T> >(rows)), nRows(rows), nCols(cols)
    {
        if (rows == 0 || cols == 0) // invalid matrix size
            throw std::runtime_error("Invalid matrix size");
        for (int i = 0; i < rows; i++)
            v[i].assign(cols, val);
    }

    T& operator() (uint8_t x, uint8_t y) 
    {
        return v[x][y];
    }

    T operator() (uint8_t x, uint8_t y) const 
    {
        return v[x][y];
    }

    // matrix multiplication
    inline SimpleMatrix<T> operator*(const SimpleMatrix<T>& B)
    {
        if (this->nCols != B.nRows)
        {
            throw std::runtime_error("Incompatible matrix sizes");
        }

        SimpleMatrix<T> temp(this->nRows, B.nCols, 0);
        
        for (int k = 0; k < this->nRows; k++)
            for (int i = 0; i < B.nCols; i++)
                for (int j = 0; j < this->nCols; j++)
                    temp.v[k][i] += this->v[k][j] * B.v[j][i];

        return temp;

    }

    inline SimpleMatrix<T> operator+(const SimpleMatrix<T>& B)
    {
        if (this->nCols != B.nCols || this->nRows != B.nRows)
        {
            throw std::runtime_error("Incompatible matrix sizes");
        }

        SimpleMatrix<T> temp(this->nRows, this->nCols, 0);

        for (int i= 0; i < this->nRows; i++)
            for (int j = 0; j < this->nCols; j++)
                    temp(i, j) = (*this)(i, j) + B(i, j);

        return temp;

    }


    // get total number of rows
    uint8_t getRows()
    {
        return nRows;
    }

    // get total number of columns
    uint8_t getCols()
    {
        return nCols;
    }

#ifdef _OSTREAM_
    friend std::ostream& operator<<(std::ostream& s, const SimpleMatrix<T>& Mat)
    {
        SimpleMatrix<T> temp = Mat;
        for (int i = 0; i < temp.getRows(); i++)
        {
            s << Mat(i, 0);
            for (int j = 1; j < temp.getCols(); j++)
                s << "," << Mat(i, j);
            s << "\n";
        }
        return s;
    }
#endif


    inline CommaInitializer<T> operator<< (const T& val)
    {
        return CommaInitializer<T>(*this, val);
    }

};*/

#pragma once

#include <vector>
#include <iostream>
#include "math.h"
#include <stdint.h>
#include <AP_Math/CommaInitializer.h>


template <typename T>
class SimpleMatrix
{

private:
    std::vector<T> v; // vector of vector to represent matrix
    uint8_t nRows, nCols;
public:

    SimpleMatrix<T>()
    {
        SimpleMatrix<T>(1, 1, 0);
    }
    // constructor from zeros
    SimpleMatrix<T>(uint8_t rows, uint8_t cols)
        : v(rows), nRows(rows), nCols(cols)
    {
    //    if (rows == 0 || cols == 0) // invalid matrix size
    //        throw std::runtime_error("Invalid matrix size");
        v.resize(rows*cols);
    }

    //constructor to fill with values val
    SimpleMatrix<T>(uint8_t rows, uint8_t cols, T val)
        : v(std::vector<T>(rows*cols)), nRows(rows), nCols(cols)
    {
    //    if (rows == 0 || cols == 0) // invalid matrix size
    //        throw std::runtime_error("Invalid matrix size");
        v.assign(rows*cols, val);
    }

    T& operator() (uint8_t x, uint8_t y)
    {
        return v[x * nCols + y];
    }

    T operator() (uint8_t x, uint8_t y) const
    {
        return v[x * nCols + y];
    }

    // matrix multiplication
    SimpleMatrix<T> operator*(const SimpleMatrix<T>& B)
    {
        if (this->nCols != B.nRows)
        {
    //        throw std::runtime_error("Incompatible matrix sizes");
        }

        SimpleMatrix<T> temp(this->nRows, B.nCols, 0);

        for (int k = 0; k < this->nRows; k++)
            for (int i = 0; i < B.nCols; i++)
                for (int j = 0; j < this->nCols; j++)
                    temp(k,i) += (*this)(k,j) * B(j,i);

        return temp;

    }

    SimpleMatrix<T>& operator*=(const T B)
    {
        for (int k = 0; k < this->nRows; k++)
            for (int i = 0; i < this->nCols; i++)
                (*this)(k, i) = B * (*this)(k, i);

        return *this;
    }


    friend SimpleMatrix<T> operator*(const T& A, const SimpleMatrix<T>& B)
    {
 
        SimpleMatrix<T> temp(B.nRows, B.nCols, 0);

        for (int k = 0; k < B.nRows; k++)
            for (int i = 0; i < B.nCols; i++)
                    temp(k, i) = A * B(k, i);

        return temp;

    }

    SimpleMatrix<T> operator+(const SimpleMatrix<T>& B)
    {
        if (this->nCols != B.nCols || this->nRows != B.nRows)
        {
    //        throw std::runtime_error("Incompatible matrix sizes");
        }

        SimpleMatrix<T> temp(this->nRows, this->nCols, 0);

        for (int i = 0; i < this->nRows; i++)
            for (int j = 0; j < this->nCols; j++)
                temp(i, j) = (*this)(i, j) + B(i, j);

        return temp;

    }


    SimpleMatrix<T> operator-(const SimpleMatrix<T>& B)
    {
        if (this->nCols != B.nCols || this->nRows != B.nRows)
        {
    //        throw std::runtime_error("Incompatible matrix sizes");
        }

        SimpleMatrix<T> temp(this->nRows, this->nCols, 0);

        for (int i = 0; i < this->nRows; i++)
            for (int j = 0; j < this->nCols; j++)
                temp(i, j) = (*this)(i, j) - B(i, j);

        return temp;

    }

    // get total number of rows
    uint8_t getRows()
    {
        return nRows;
    }

    // get total number of columns
    uint8_t getCols()
    {
        return nCols;
    }

#ifdef _OSTREAM_
    friend std::ostream& operator<<(std::ostream& s, const SimpleMatrix<T>& Mat)
    {
        SimpleMatrix<T> temp = Mat;
        for (int i = 0; i < temp.getRows(); i++)
        {
            s << Mat(i, 0);
            for (int j = 1; j < temp.getCols(); j++)
                s << "," << Mat(i, j);
            s << "\n";
        }
        return s;
    }
#endif


    inline CommaInitializer<T> operator<< (const T& val)
    {
        return CommaInitializer<T>(*this, val);
    }

};