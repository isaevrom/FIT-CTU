#ifndef REFACTORED_CMERGE_HPP
#define REFACTORED_CMERGE_HPP

#include "CBinaryOperation.hpp"
#include <string>

class CMerge : public CBinaryOperation {
 public:
    ~CMerge() override = default;
    /**
     * Reads context for execution (e.g.: name of a matrix to be saved into the database)
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;

    /**
     * Multiplies two matrices and saves it as a new matrix into the database
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
 private:
    /**
     * Either "H" or "V"
     * H -- horizontal merging
     * V -- vertical merging
     */
    std::string mergingOperation;
};


#endif //REFACTORED_CMERGE_HPP
