#ifndef REFACTORED_CCUT_HPP
#define REFACTORED_CCUT_HPP

#include "CUnaryOperation.hpp"
#include <string>

class CCut : public CUnaryOperation {
public:
    ~CCut() override = default;

    /**
     * Cuts matrix from the database and saves the result as a new matrix
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;

    /**
     * Reads context for execution (e.g.: name of a matrix to be saved into the database)
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;

    /**
     * Used to set boundaries of a cut matrix
     * @param b
     * @param description
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool SetBoundary(int & b, const std::string & description);
 private:
    /**
     * rS -- starting row    boundary
     * rE -- ending   row    boundary
     * cS -- starting column boundary
     * cE -- ending   column boundary
     */
    int rS, rE,
        cS, cE;
};


#endif //REFACTORED_CCUT_HPP
