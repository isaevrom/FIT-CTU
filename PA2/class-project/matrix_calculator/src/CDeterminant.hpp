#ifndef REFACTORED_CDETERMINANT_HPP
#define REFACTORED_CDETERMINANT_HPP

#include "CUnaryOperation.hpp"
#include "CDatabase.hpp"

class CDeterminant : public CUnaryOperation {
 public:
    ~CDeterminant() override = default;
    /**
     * Reads the name of a matrix which determinant user wants to find out
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;
    /**
     * Finds determinant of a square matrix
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
};

#endif //REFACTORED_CDETERMINANT_HPP
