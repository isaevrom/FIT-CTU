#include "COperation.hpp"
#include "CUnaryOperation.hpp"
#include "CTranspose.hpp"
#include "CMatrixHolder.hpp"
#include <iostream>
#include <string>
#include <new>

using namespace std;

void CTranspose::Execute(CDatabase & matrixDatabase) {
    if (! matrixDatabase.IsPresent(sourceName)) {
        cout << "No matrix called '" << sourceName << "' in the database." << endl;
        cout << "Please, check out list of names of matrices using LIST operation." << endl;
        return;
    }
    int tmpRows = matrixDatabase.AccessMatrix(sourceName)->GetRows();
    int tmpCols = matrixDatabase.AccessMatrix(sourceName)->GetCols();
    // reversed order of passing arguments, because we want to create a transposition
    CMatrixHolder matrixHolder(tmpCols, tmpRows);

    for (int row = 0; row < tmpRows; row++)
        for (int col = 0; col < tmpCols; col++)
            for (int k = 0; k < matrixDatabase.ColsNumber(sourceName); k++)
                matrixHolder.SetCoord(col, row)
                         =
                matrixDatabase.AccessMatrix(sourceName)->GetCoord(row, col);
    try {
        matrixDatabase.AddMatrix(newName, matrixHolder.CreateMatrix());
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to add transposed matrix" << endl;
        throw;
    }
    matrixDatabase.AccessMatrix(newName)->Print();
}

