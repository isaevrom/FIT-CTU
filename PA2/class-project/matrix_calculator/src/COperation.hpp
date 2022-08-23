#ifndef REFACTORED_COPERATION_HPP
#define REFACTORED_COPERATION_HPP

#include "CDatabase.hpp"
#include <string>

/**
 * This class is a parent of all the operations
 * operations are (typically) used to add new matrices
 * into the database they receive as a parameter via Execute method.
 * They are also used to find out interesting facts about matrices (determinant, rank)
 */
class COperation {
 public:
    virtual ~COperation() = default;

    /**
     * Executes matrix database in some way
     * @param matrixDatabase
     */
    virtual void Execute(CDatabase & matrixDatabase) = 0;

    /**
     * Reads context for execution (e.g.: name of a matrix to be saved into the database)
     * @return false if reading via getline fails in some way, true otherwise
     */
    virtual bool ReadContext() = 0;
 protected:
    /**
     * Name of a new matrix to be saved into the database
     */
    std::string newName;

    /**
     * Sets name of an operand, is seen by descendant classes only
     * @param operand
     * @param message
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool SetOperandName(std::string & operand, const std::string & message);
};

#endif //REFACTORED_COPERATION_HPP
