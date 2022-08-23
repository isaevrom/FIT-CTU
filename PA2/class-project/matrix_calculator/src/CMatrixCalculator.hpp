#ifndef REFACTORED_CMATRIXCALCULATOR_HPP
#define REFACTORED_CMATRIXCALCULATOR_HPP

#include "COperation.hpp"
#include "CDatabase.hpp"
#include <memory>
#include <set>
#include <string>

class CMatrixCalculator {
 public:
    /**
     * Constructor constructing the database of calculator
     */
    CMatrixCalculator() = default;

    /**
     * Read matrix name
     * @param name
     * @return true if reading is successful (i.e. some word is read), false if something goes wrong with getline function
     */
    static bool ReadName(std::string &name);

    /**
     * Read name of an operation
     * @param opName
     * @return true if reading is successful (i.e. some word is read), false if something goes wrong with getline function
     */
    static bool ReadOperation(std::string & opName);

    /**
     * Instantiates new operation
     * @param opName
     * @return shared pointer of an operation which is demanded by the user
     */
    std::shared_ptr<COperation> LoadOperation(const std::string & opName);

    /**
     * Lists calculator's operations
     */
    static void ListOperations();
    /**
     * Runs calculator
     */
    void Run();
 private:
    /**
     * Calculator's database of matrices
     */
    CDatabase matrixDatabase;
    /**
     * List of calculator's operations
     */
    static const std::set<std::string> opList;
    /**
     * List of allowed non-alpha symbols in operations
     */
    static const std::set<char>        opSymbols;
};


#endif //REFACTORED_CMATRIXCALCULATOR_HPP
