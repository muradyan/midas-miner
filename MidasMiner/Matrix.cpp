
#include "Matrix.h"

#include <cassert>

Matrix::Matrix()
{
}

Matrix::color Matrix::element(index_t row, index_t column) const
{
    assert(row < numberOfRows);
    assert(column < numberOfColumns);
    return m_matrix[row][column];
}


Matrix::color& Matrix::element(index_t row, index_t column)
{
    assert(row < numberOfRows);
    assert(column < numberOfColumns);
    return m_matrix[row][column];
}
