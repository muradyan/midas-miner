
#include "Control.h"

#include <cassert>
#include <ctime>
#include <cstdlib>

Control::Control()
{
    srand(static_cast<unsigned int>(time(nullptr))); 
}

Control::range_t Control::computeLongestCommonRangeInRow()
{
    range_t max;
    for (Matrix::index_t i = 0; i < Matrix::numberOfRows; ++i) {
        Matrix::index_t start = 0;
        Matrix::color common = m_matrix.element(i, start);
        Matrix::index_t j = 0;
        for (; j < Matrix::numberOfColumns; ++j) {
            if (m_matrix.element(i, j) != common) {
                if (j - start > max.length()) {
                    max.begin = start;
                    max.end = j;
                    max.index = i;
                }
                start = j;
                common = m_matrix.element(i, start);
            } 
        }
        if (j - start > max.length()) {
            max.begin = start;
            max.end = j;
            max.index = i;
        }
    }
    return max;
}

Control::range_t Control::computeLongestCommonRangeInColumn()
{
    range_t max;
    for (Matrix::index_t j = 0; j < Matrix::numberOfColumns; ++j) {
        Matrix::index_t start = 0;
        Matrix::color common = m_matrix.element(start, j);
        Matrix::index_t i = 0;
        for (; i < Matrix::numberOfRows; ++i) {
            if (m_matrix.element(i, j) != common) {
                if (i - start > max.length()) {
                    max.begin = start;
                    max.end = i;
                    max.index = j;
                }
                start = i;
                common = m_matrix.element(start, j);
            }
        }
        if (i - start > max.length()) {
            max.begin = start;
            max.end = i;
            max.index = j;
        }
    }
    return max;
}

void Control::deductRow(const Control::range_t& range)
{
    assert(range.length() >= minimumDeductible);
    for (Matrix::index_t i = range.index; i > 0; --i) {
        for (Matrix::index_t j = range.begin; j < range.end; ++j) {
            m_matrix.element(i, j) = m_matrix.element(i - 1, j);
        }
    }
    for (Matrix::index_t j = range.begin; j < range.end; ++j) {
        m_matrix.element(0, j) = generateRandomColor();
    }
}

void Control::deductColumn(const Control::range_t& range)
{
    assert(range.length() >= minimumDeductible);
    for (Matrix::index_t j = range.index; j > 0; --j) {
        for (Matrix::index_t i = range.begin; i < range.end; ++i) {
            m_matrix.element(i, j) = m_matrix.element(i, j - 1);
        }
    }
    for (Matrix::index_t i = range.begin; i < range.end; ++i) {
        m_matrix.element(i, 0) = generateRandomColor();
    }
}

Matrix::color Control::generateRandomColor() const
{
    return static_cast<Matrix::color>(rand() % Matrix::max_color);
}

void Control::generateRandomMatrix()
{
    for (size_t i = 0; i < Matrix::numberOfRows; ++i) {
        for (size_t j = 0; j < Matrix::numberOfColumns; ++j) {
            m_matrix.element(i, j) = generateRandomColor();
        }
    }
}

void Control::init()
{
    generateRandomMatrix();
    while (deduct());
}

/// @todo calling this function in a loop in the UI thread may result 
/// the UI thread to freeze sometimes.  If noticable run the loop in a separate thread
/// @todo or if the delay is too long optimization step should be considered to run
/// the algorithm only on the potentially deductable area
/// here emphasis is put on simplicity rather than writing sophisticated algorithm
bool Control::deduct()
{
    range_t row = computeLongestCommonRangeInRow();
    range_t column = computeLongestCommonRangeInColumn();
    if (row.length() >= column.length() && row.length() >= minimumDeductible) {
        deductRow(row);
    } else if (column.length() >= minimumDeductible) {
        deductColumn(column);
    } else {
        return false;
    }
    return true;
}

bool Control::adjacent(Matrix::index_t row1, 
       Matrix::index_t column1,
       Matrix::index_t row2,
       Matrix::index_t column2)
{
    Matrix::index_t rowDifference = (row1 > row2) ? (row1 - row2) : (row2 - row1);
    Matrix::index_t columnDifference = (column1 > column2) ? (column1 - column2) : (column2 - column1);
    return (rowDifference + columnDifference == 1);
}

bool Control::trySwap(Matrix::index_t row1, 
       Matrix::index_t column1,
       Matrix::index_t row2,
       Matrix::index_t column2)
{
    assert(row1 < Matrix::numberOfRows);
    assert(row2 < Matrix::numberOfRows);
    assert(column1 < Matrix::numberOfColumns);
    assert(column2 < Matrix::numberOfColumns);
    if (!adjacent(row1, column1, row2, column2)) {
        return false;
    }
    swap(row1, column1, row2, column2);
    if (computeLongestCommonRangeInRow().length() < minimumDeductible 
        && computeLongestCommonRangeInColumn().length() < minimumDeductible) {
        swap(row1, column1, row2, column2);
        return false;
    } 
    return true;
}

void Control::swap(Matrix::index_t row1, 
       Matrix::index_t column1,
       Matrix::index_t row2,
       Matrix::index_t column2)
{
    Matrix::color c = m_matrix.element(row1, column1);
    m_matrix.element(row1, column1) = m_matrix.element(row2, column2);
    m_matrix.element(row2, column2) = c;
}

const Matrix& Control::getMatrix() const
{
    return m_matrix;
}
