#ifndef REFACTORED_CIDENTITY_HPP
#define REFACTORED_CIDENTITY_HPP

#include "CUnaryOperation.hpp"
#include "CMatrixHolder.hpp"

class CIdentity : public CUnaryOperation {
 public:
    ~CIdentity() override = default;
    /**
     * Creates identity matrix and saves it into the database
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
    /**
     * Reads context for execution (e.g.: name of a matrix to be saved into the database)
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;
 private:
    /**
     * Metric is a size N of an NxN identity matrix
     */
    int metric;
};

#endif //REFACTORED_CIDENTITY_HPP
