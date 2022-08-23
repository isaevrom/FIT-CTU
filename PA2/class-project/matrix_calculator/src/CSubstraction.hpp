#ifndef REFACTORED_CSUBSTRACTION_HPP
#define REFACTORED_CSUBSTRACTION_HPP

#include "CBinaryOperation.hpp"
#include "CDatabase.hpp"

class CSubstraction : public CBinaryOperation {
 public:
    ~CSubstraction() override = default;

    /**
     * Executes substraction of two matrices from the database and saves the result as a new matrix
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
 private:
};

#endif //REFACTORED_CSUBSTRACTION_HPP
