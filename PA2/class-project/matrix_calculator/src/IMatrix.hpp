#ifndef REFACTORED_IMATRIX_HPP
#define REFACTORED_IMATRIX_HPP

#include <iostream>

class IMatrix {
 public:
    /**
     * Constructor, sets number of rows and columns of the matrix
     * @param rows
     * @param cols
     */
    IMatrix(int rows, int cols);

    virtual ~IMatrix() = default;

    int GetRows() const;
    int GetCols() const;

    virtual double GetCoord(int row, int col) const = 0;

    /**
     * Saves matrix on a disk
     * @param name
     * @return false if writing data into the disk fails, true otherwise
     */
    virtual bool SaveMatrix(const std::string & name) const = 0;

    /**
     * Prints matrix
     */
    virtual void   Print() const = 0;
protected:
    int rows, cols;
};

#endif //REFACTORED_IMATRIX_HPP
