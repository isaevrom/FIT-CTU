#ifndef REFACTORED_CDENSEMATRIX_HPP
#define REFACTORED_CDENSEMATRIX_HPP

#include "IMatrix.hpp"
#include <vector>

class CDenseMatrix : public IMatrix {
public:
    /**
     * Constructor of dense matrix, copies 2D vector matrix
     * @param matrix
     * @param rows
     * @param cols
     */
    CDenseMatrix(const std::vector<std::vector<double>> & matrix, int rows, int cols);

    ~CDenseMatrix() override = default;

    double GetCoord(int row, int col) const override;

    /**
     * Saves matrix on a disk
     * @param name
     * @return false if writing data into the disk fails, true otherwise
     */

    bool SaveMatrix(const std::string & name) const override;

    /**
     * Prints matrix
     */
    void Print() const override;
private:
    /**
     * Dense matrix is stored in a 2D vector
     */
    std::vector<std::vector<double>> matrix;
};


#endif //REFACTORED_CDENSEMATRIX_HPP
