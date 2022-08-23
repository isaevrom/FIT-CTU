#ifndef REFACTORED_CSPARSEMATRIX_HPP
#define REFACTORED_CSPARSEMATRIX_HPP

#include "IMatrix.hpp"
#include <utility>
#include <map>
#include <vector>

class CSparseMatrix : public IMatrix {
public:
    /**
     * Constructs sparse matrix from a 2D vector
     * @param matrix
     * @param rows
     * @param cols
     */
     CSparseMatrix(const std::vector<std::vector<double>> & matrix, int rows, int cols);

    ~CSparseMatrix() override = default;

    double   GetCoord(int row, int col) const override;

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
     * Sparse matrix is stored efficiently in a map, only non-zero coordinate values are stored
     */
    std::map<std::pair<int,int>, double> matrix;
};



#endif //REFACTORED_CSPARSEMATRIX_HPP
