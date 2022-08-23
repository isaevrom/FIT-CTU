#ifndef REFACTORED_CADDITION_HPP
#define REFACTORED_CADDITION_HPP

#include "CBinaryOperation.hpp"
#include <string>

/**
 * Class used to provide addition of matrices
 */
class CAddition : public CBinaryOperation {
 public:
    ~CAddition() override = default;

    /**
     * Executes addition of two matrices from the database and saves the result as a new matrix
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
};


#endif //REFACTORED_CADDITION_HPP
