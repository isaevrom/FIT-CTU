#ifndef REFACTORED_CGEM_HPP
#define REFACTORED_CGEM_HPP

#include "CUnaryOperation.hpp"

class CGEM : public CUnaryOperation {
public:
    ~CGEM() override = default;

    /**
     * Sets isDetailed to true if user wants to see steps of GEM, to false otherwise
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadDetails();

    /**
     * Reads context for execution (e.g.: name of a matrix to be saved into the database)
     * @return false if reading via getline fails in some way, true otherwise
     */
    bool ReadContext() override;

    /**
     * GEMs a matrix from the database and saves it as a new one
     * @param matrixDatabase
     */
    void Execute(CDatabase & matrixDatabase) override;
 private:
    /**
     * Indicates whether user wants to see steps of GEM
     */
    bool isDetailed;
};

#endif //REFACTORED_CGEM_HPP
