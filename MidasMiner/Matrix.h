
#ifndef MATRIX_H
#define MATRIX_H

/// data model representing the content on the board
class Matrix 
{
public:
    // index type
    typedef size_t index_t;

public:
    static const index_t numberOfRows = 8;
    static const index_t numberOfColumns = 8;

public:

    /// default constructor
    Matrix();

    enum color
    {
        blue = 0,
        green = 1,
        purple = 2,
        red = 3,
        yellow = 4,
        max_color = 5,
    };

    /// generates random matrix using all possible colors
    void generateRandom();

    /// returns the color for the given coordinate
    /// @pre column < 8 && row < 8
    color element(index_t row, index_t column) const;

    /// returns reference to the color for the given coordinate
    /// row < Matrix::numberOfRows && column < Matrix::numberOfColumns
    color& element(index_t row, index_t column);

private:
    color m_matrix[numberOfRows][numberOfColumns];

private:
    Matrix(const Matrix&);
    Matrix& operator=(const Matrix&);
};
 
#endif // MATRIX_H
