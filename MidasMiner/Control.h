
#ifndef CONTROL_H
#define CONTROL_H

#include "Matrix.h"

#include <limits>

/// represents the ViewModel or Control of the game 
class Control
{
private:
    // size of minimum deductable objects in a row/column
    static const size_t minimumDeductible = 3;

public:
    // one game lasts 60 seconds
    static const size_t gameDuration = 60;

public:
    /// constructor 
    Control();

    /// @brief initializes the game 
    /// @details non deductible random matrix is generated
    void init();

    /** 
    @brief makes one step. 
    @details Objects can be switched with other adjacent objects:
        - If this switch results in 3 or more objects of the same 
          colour in a row or column, these objects are removed from the game
        - If the switch does not result in a match, the 
          switched objects are returned to their original position
    @pre row1 < Matrix::numberOfRows && col1 < Matrix::numberOfColumns
    @pre row2 < Matrix::numberOfRows && col2 < Matrix::numberOfColumns
    @param row1 the row of the first object to be switched 
    @param column1 the column of the first object to be switched 
    @param row2 the row of the second object to be switched 
    @param column2 the column of the second object to be switched 
    @return returns true if swap happened - deduction should follow
    */
    bool trySwap(Matrix::index_t row1, 
           Matrix::index_t column1,
           Matrix::index_t row2,
           Matrix::index_t column2);

    /**
    @brief finds deducible elements in row/column erases them and fills with new elements
    @details When objects are removed from the game, existing objects with nothing 
    beneath them fall down whilst new objects fall in from the top to fill in the blank spaces
    @return false if deduction is not performed (already completely deducted)
    */
    bool deduct();

    /// returns const Matrix object
    const Matrix& getMatrix() const;

private:
    struct range_t
    {
        // row or column index
        Matrix::index_t index;

        // range begin
        Matrix::index_t begin;

        // range end
        Matrix::index_t end;

        // retruns the length of the range
        Matrix::index_t length() const { return end - begin; }

        range_t() 
            : index(std::numeric_limits<Matrix::index_t>::max())
            , begin(0)
            , end(0) {}
    };

private:
    bool adjacent(Matrix::index_t row1, 
       Matrix::index_t column1,
       Matrix::index_t row2,
       Matrix::index_t column2);

    void swap(Matrix::index_t row1, 
       Matrix::index_t column1,
       Matrix::index_t row2,
       Matrix::index_t column2);

    range_t computeLongestCommonRangeInRow();
    range_t computeLongestCommonRangeInColumn();
    void deductRow(const Control::range_t& range);
    void deductColumn(const Control::range_t& range);
    void generateRandomMatrix();
    Matrix::color generateRandomColor() const;

private:
    Control(const Control&);
    Control& operator=(const Control&);

private:
    Matrix m_matrix;
};

#endif // CONTROL_H