#ifndef REFACTORED_CUNARYOPERATION_HPP
#define REFACTORED_CUNARYOPERATION_HPP

#include "COperation.hpp"
#include <string>

/**
 * Class used as a parent to unary operations, such as scan, identity, cut, ...
 */
class CUnaryOperation : public COperation {
 public:
    ~CUnaryOperation() override = default;
    /**
     * Reads context for execution (e.g.: name of a matrix to be saved into the database)
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;
 protected:
    /**
     * Name of the matrix unary operations operate with
     */
    std::string sourceName;
};


#endif //REFACTORED_CUNARYOPERATION_HPP
