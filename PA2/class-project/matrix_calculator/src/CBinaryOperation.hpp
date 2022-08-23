#ifndef REFACTORED_CBINARYOPERATION_HPP
#define REFACTORED_CBINARYOPERATION_HPP

#include "COperation.hpp"
#include <string>

/**
 * Class is a parent of binary operations, such as addition, multiplication, ...
 */
class CBinaryOperation : public COperation {
 public:
    ~CBinaryOperation() override = default;
    /**
     * Reads context for execution (i.e.: name of a matrix to be saved into the database, name of the operands)
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;
 protected:
    /**
     * Binary operations must have 2 operands, hence there are two names of the matrices we operate with
     */
    std::string nameA, nameB;
};

#endif //REFACTORED_CBINARYOPERATION_HPP
