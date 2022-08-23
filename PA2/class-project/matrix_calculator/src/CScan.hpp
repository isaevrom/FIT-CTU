#ifndef REFACTORED_CSCAN_HPP
#define REFACTORED_CSCAN_HPP

#include "CUnaryOperation.hpp"
#include "CDatabase.hpp"
#include "CMatrixHolder.hpp"

class CScan : public CUnaryOperation {
 public:
    ~CScan() override = default;

    /**
     *
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;

    /**
     * Reads each row of a new matrix
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadRows();

    /**
     * Reads context for execution (e.g.: name of a matrix to be saved into the database)
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;
 private:
    /**
     * Number of rows & cols of a new matrix
     */
    int rows, cols;

    /**
     * Temporary matrix data storage, which instantiates the true matrix in during the execution
     */
    CMatrixHolder matrixHolder;
};


#endif //REFACTORED_CSCAN_HPP
