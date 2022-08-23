#ifndef REFACTORED_CINVERSE_HPP
#define REFACTORED_CINVERSE_HPP

#include "CUnaryOperation.hpp"

class CInverse : public CUnaryOperation {
 public:
    ~CInverse() override = default;
    /**
     * Inverses a particular square matrix and saves it as a new one into the database
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
};


#endif //REFACTORED_CINVERSE_HPP
