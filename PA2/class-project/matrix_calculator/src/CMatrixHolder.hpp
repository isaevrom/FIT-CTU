#ifndef REFACTORED_CMATRIXHOLDER_HPP
#define REFACTORED_CMATRIXHOLDER_HPP

#include "IMatrix.hpp"
#include <vector>
#include <memory>

/**
 * Serves as a "middleclass", i.e. it is used to hold matrix temporarily, can instantiate matrices
 */

class CMatrixHolder {
public:
    /**
     * Sets rows and columns, calls Expand() method which sets matrix with zeros
     * @param rows
     * @param cols
     */
    CMatrixHolder(int rows, int cols);
     CMatrixHolder() = default;
    ~CMatrixHolder() = default;

    /**
     * Sets matrix with zeros
     */
    void Expand();

    double   GetCoord(int row, int col) const;
    double & SetCoord(int row, int col);

    int GetRows() const;
    int GetCols() const;

    void SetRows(int rows);
    void SetCols(int cols);

    /**
     * Gets density
     * @return number of non-zero coordinates of a matrix
     */
    int GetDensity() const;

    /**
     * Instantiates matrix
     * @return shared CSparseMatrix pointer if matrix is sparse, CDenseMatrix otherwise
     */
    std::shared_ptr<IMatrix> CreateMatrix() const;

    void Print() const;
private:
    /**
     * Number of rows and cols of a matrix
     */
    int rows, cols;
    /**
     * Matrix is stored in that 2D vector
     */
    std::vector<std::vector<double>> matrixHolder;
};



#endif //REFACTORED_CMATRIXHOLDER_HPP
