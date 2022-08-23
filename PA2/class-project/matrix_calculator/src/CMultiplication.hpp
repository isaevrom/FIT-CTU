#ifndef REFACTORED_CMULTIPLICATION_HPP
#define REFACTORED_CMULTIPLICATION_HPP

#include "CBinaryOperation.hpp"

class CMultiplication : public CBinaryOperation {
 public:
    ~CMultiplication() override = default;
    /**
     * Multiplies two matrices and saves it as a new matrix into the database
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
 private:
};

#endif //REFACTORED_CMULTIPLICATION_HPP
