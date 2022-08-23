#include "CAddition.hpp"
#include "CDatabase.hpp"
#include "IMatrix.hpp"
#include "CMatrixHolder.hpp"
#include <iostream>
#include <memory>
#include <new>

using namespace std;

void CAddition::Execute(CDatabase & matrixDatabase) {
    cout << "CAddition's execute" << endl;

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

    if (    matrixDatabase.RowsNumber(nameA) != matrixDatabase.RowsNumber(nameB)
                ||
            matrixDatabase.ColsNumber(nameA) != matrixDatabase.ColsNumber(nameB)) {
        cout << "Can't do addition upon matrices with different metrics!" << endl;
        cout << "Please, check out metrics of a matrix that interests you using METRICS operation." << endl;
        return;
    }

    int tmpRows = matrixDatabase.RowsNumber(nameA);
    int tmpCols = matrixDatabase.ColsNumber(nameA);

    CMatrixHolder matrixHolder(tmpRows, tmpCols);

    shared_ptr<IMatrix> A = matrixDatabase.AccessMatrix(nameA);
    shared_ptr<IMatrix> B = matrixDatabase.AccessMatrix(nameB);

    for ( int row = 0; row < tmpRows; row++ ) {
        for ( int col = 0; col < tmpCols; col++ )
            matrixHolder.SetCoord(row, col) =
                    A->GetCoord(row, col)
                             +
                    B->GetCoord(row, col);
    }
    try {
        matrixDatabase.AddMatrix(newName, matrixHolder.CreateMatrix());
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to store + matrix" << endl;
        throw;
    }
    cout << "Resulting matrix:" << endl;
    matrixDatabase.PrintMatrix(newName);
}
