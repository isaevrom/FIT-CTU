#ifndef REFACTORED_CRANK_HPP
#define REFACTORED_CRANK_HPP

#include "CUnaryOperation.hpp"

class CRank : public CUnaryOperation {
 public:
    ~CRank() override = default;

    /**
     * Reads the name of a matrix which rank user wants to find out
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;

    /**
     * Finds rank of some matrix from the database
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
};


#endif //REFACTORED_CRANK_HPP
