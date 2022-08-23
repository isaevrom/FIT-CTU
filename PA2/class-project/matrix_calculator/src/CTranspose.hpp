#ifndef REFACTORED_CTRANSPOSE_HPP
#define REFACTORED_CTRANSPOSE_HPP

#include "CUnaryOperation.hpp"

class CTranspose : public CUnaryOperation {
 public:
    ~CTranspose() override = default;

    /**
     * Transposes an existing matrix and saves the result as a new matrix
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
 private:
};


#endif //REFACTORED_CTRANSPOSE_HPP
