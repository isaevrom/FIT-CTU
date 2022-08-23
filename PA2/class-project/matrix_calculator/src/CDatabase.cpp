#include "CDatabase.hpp"
#include "CMatrixHolder.hpp"
#include <map>
#include <string>
#include <memory>
#include <exception>
#include <fstream>
#include <new>

using namespace std;

void CDatabase::AddMatrix(const std::string & name, std::shared_ptr<IMatrix> matrixToAdd) {
    try {
        matrixDatabase[name] = matrixToAdd;
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to add matrix into the database" << endl;
        throw;
    }
}

bool CDatabase::LoadMatrix(const std::string & matrixToLoad, const std:: string & saveAsName) {
    string fileName = "matrix_" + matrixToLoad;
    ifstream inFile( fileName );
    if ( !inFile ) { cout << "File " << fileName << " could not be opened." << endl; return false; }

    string typeOfMatrix;

    if ( !( inFile >> typeOfMatrix) ) {
        cout << "Not a valid file!" << endl;
        return false;
    }

    int rows, cols;

    if ( ! (inFile >> rows >> cols) ) {
        cout << "Not a valid file!" << endl;
        return false;
    }
    if ( rows <= 0 || cols <= 0 ) {
        cout << "Not a valid file!" << endl;
        return false;
    }

    CMatrixHolder matrixHolder(rows, cols);

    if ( typeOfMatrix == "sparse" ) {
        int coordsAmount;
        if ( ! (inFile >> coordsAmount) ) {
            cout << "Not a valid file!" << endl;
            return false;
        }

        int r, c;
        double value;

        while (coordsAmount) {
            if ( ! (inFile >> r >> c >> value) ) {
                cout << "Not a valid file!" << endl;
                return false;
            }
            if ( r < 0 || r >= rows  || c < 0 || c >= cols ) {
                cout << "Not a valid file!" << endl;
                return false;
            }
            matrixHolder.SetCoord(r, c) = value;
            --coordsAmount;
        }
    } else if ( typeOfMatrix == "dense" ) {
        double value;
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++) {
                if (!(inFile >> value)) {
                    cout << "Not a valid file!" << endl;
                    return false;
                }
                matrixHolder.SetCoord(r, c) = value;
            }
    } else {
        cout << "Not a valid file!" << endl;
        return false;
    }

    string emptyString;
    inFile >> emptyString;
    if ( ! emptyString.empty() ) {
        cout << "Not a valid file!" << endl;
        return false;
    }
    try {
        AddMatrix(saveAsName, matrixHolder.CreateMatrix());
    } catch (const bad_alloc & ba) {
        inFile.close();
        cout << "Not enough memory to store loaded matrix." << endl;
        throw;
    }
    inFile.close();
    return true;
}

std::shared_ptr<IMatrix> CDatabase::AccessMatrix(const std::string & name) const {
    if (matrixDatabase.count(name) == 0)
        throw out_of_range("CDatabase::AccessMatrix -- attempting to access out of range data.");
    return matrixDatabase.at(name);
}

void CDatabase::DeleteMatrix(const std::string & name) {
    if ( matrixDatabase.count(name) == 0 ) {
        cout << "No matrix called " << "'" << name << "'" << " in the database." << endl;
        return;
    }
    cout << "Deleting matrix " << name << " from the database." << endl;
    matrixDatabase.erase(name);
}

bool CDatabase::IsPresent(const std::string & name) const {
    if (matrixDatabase.count(name) == 0)
        return false;
    return true;
}

int CDatabase::RowsNumber(const std::string & name) const {
    if (matrixDatabase.count(name) == 0)
        throw out_of_range("CDatabase::RowsNumber -- attempting to access out of range data.");
    return matrixDatabase.at(name)->GetRows();
}

int CDatabase::ColsNumber(const std::string & name) const {
    if (matrixDatabase.count(name) == 0)
        throw out_of_range("CDatabase::ColsNumber -- attempting to access out of range data.");
    return matrixDatabase.at(name)->GetCols();
}

void CDatabase::ListMatrices() const {
    cout << "List of the currently present matrices:" << endl;
    cout << "#------------------------------" << endl;
    for (const auto & it : matrixDatabase)
        cout << "\t\t" << it.first << endl;
    if (matrixDatabase.empty())
        cout << "\t\t  empty list" << endl;
    cout << "------------------------------#" << endl;
}

void CDatabase::PrintMatrix(const std::string & name) const {
    if ( matrixDatabase.count(name) > 0 ) {
        cout << "#------------------------------" << endl;
        cout << "Matrix:\t" << name << endl;
        cout << "Size:\t" << RowsNumber(name) << "r x " << ColsNumber(name) << "c" << endl;
        cout << "Representation:" << endl;
        matrixDatabase.at(name)->Print();
        cout << "------------------------------#" << endl;
    }
    else
        cout << "No matrix called " << "'" << name << "'" << " in the database." << endl;
}