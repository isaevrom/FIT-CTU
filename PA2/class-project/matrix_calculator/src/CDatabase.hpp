#ifndef REFACTORED_CDATABASE_HPP
#define REFACTORED_CDATABASE_HPP

#include "IMatrix.hpp"
#include <memory>
#include <map>
#include <string>

class CDatabase {
 public:
    /**
     * Adds matrix called name into the database
     * @param name
     * @param matrixToAdd
     */
    void AddMatrix(const std::string & name, std::shared_ptr<IMatrix> matrixToAdd);

    /**
     * Deletes matrix matrixToDelete
     * @param matrixToDelete
     */
    void DeleteMatrix(const std::string & matrixToDelete);

    /**
     * Loads matrix from a file
     * @param matrixToLoad
     * @param saveAsName
     * @return true if reading is successful, false if something is wrong with the user's disk
     */
    bool LoadMatrix(const std::string & matrixToLoad, const std:: string & saveAsName);

    /**
     * Checks whether matrix called name exists in the database
     * @param name
     * @return true if it does, false if it doesn't exist
     */
    bool IsPresent(const std::string & name) const;

    /**
     * Provides the option of accessing information of a particular matrix
     * @param name
     * @return pointer to matrix called name
     */
    std::shared_ptr<IMatrix> AccessMatrix(const std::string & name) const;

    int RowsNumber(const std::string & name) const;

    int ColsNumber(const std::string & name) const;

    void ListMatrices() const;

    void PrintMatrix(const std::string & name) const;
 private:
    /**
     * String serves as a matrix name which leads to the actual matrix
     */
    std::map<std::string, std::shared_ptr<IMatrix>> matrixDatabase;
};

#endif //REFACTORED_CDATABASE_HPP
