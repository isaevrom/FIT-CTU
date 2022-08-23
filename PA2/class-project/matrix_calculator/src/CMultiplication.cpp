#include "CMultiplication.hpp"
#include "CMatrixHolder.hpp"
#include <iostream>
#include <new>

using namespace std;

void CMultiplication::Execute(CDatabase &matrixDatabase) {
    if (! matrixDatabase.IsPresent(nameA) ) {
        cout << "No matrix called '" << nameA << "' in the database." << endl;
        cout << "Please, check out list of names of matrices using LIST operation." << endl;
        return;
    }

    if (! matrixDatabase.IsPresent(nameB)) {
        cout << "No matrix called '" << nameB << "' in the database." << endl;
        cout << "Please, check out list of names of matrices using LIST operation." << endl;
        return;
    }

    if (matrixDatabase.ColsNumber(nameA) != matrixDatabase.RowsNumber(nameB)) {
        cout << "Can't multiply matrices with different metrics!" << endl;
        cout << "Please, check out metrics of a matrix that interests you using METRICS operation." << endl;
        return;
    }

    int tmpRows = matrixDatabase.RowsNumber(nameA);
    int tmpCols = matrixDatabase.ColsNumber(nameB);

    CMatrixHolder matrixHolder(tmpRows, tmpCols);

    for (int row = 0; row < tmpRows; row++) {
        for (int col = 0; col < tmpCols; col++)
            for (int k = 0; k < matrixDatabase.ColsNumber(nameA); k++)
                matrixHolder.SetCoord(row, col) +=
                        matrixDatabase.AccessMatrix(nameA)->GetCoord(row, k)
                            *
                        matrixDatabase.AccessMatrix(nameB)->GetCoord(k, col);
    }

    try {
        matrixDatabase.AddMatrix(newName, matrixHolder.CreateMatrix());
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to add * matrix" << endl;
        throw;
    }
    cout << "Resulting matrix:" << endl;
    matrixDatabase.PrintMatrix(newName);
}
