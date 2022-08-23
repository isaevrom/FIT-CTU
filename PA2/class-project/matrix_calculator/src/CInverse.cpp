#include "CInverse.hpp"
#include "CMatrixHolder.hpp"
#include <iostream>
#include <new>

using namespace std;

void CInverse::Execute(CDatabase &matrixDatabase) {
    if (! matrixDatabase.IsPresent(sourceName)) {
        cout << "No matrix called '" << sourceName << "' in the database." << endl;
        cout << "Please, check out list of names of matrices using LIST operation." << endl;
        return;
    }
    if ( matrixDatabase.RowsNumber(sourceName) != matrixDatabase.ColsNumber(sourceName) ) {
        cout << sourceName << " is not a square matrix." << endl;
        return;
    }

    CMatrixHolder matrixHolder(matrixDatabase.RowsNumber(sourceName),
                               matrixDatabase.ColsNumber(sourceName));
    for (int row = 0; row < matrixHolder.GetRows(); row++)
        for (int col = 0; col < matrixHolder.GetCols(); col++)
            matrixHolder.SetCoord(row, col) = matrixDatabase.AccessMatrix(sourceName)->GetCoord(row, col);


    CMatrixHolder inverseHolder(matrixDatabase.RowsNumber(sourceName),
                                matrixDatabase.ColsNumber(sourceName));
    for (int row = 0; row < matrixHolder.GetRows(); row++)
        inverseHolder.SetCoord(row, row) = 1;

    int l = 0, k = 0;
    int rows = matrixHolder.GetRows();
    int cols = matrixHolder.GetCols();

    bool fullZeroColumn;
    double nullifyingValue;

    while ( l < rows && k < cols ) {
        fullZeroColumn = true;
        for ( int j = l; j < rows; j++ ) {
            if ( matrixHolder.GetCoord(j, k) != 0.0 ) {
                fullZeroColumn = false;
                if (matrixHolder.GetCoord(l, k) == 0.0) {
                    for (int col = 0; col < cols; col++) {
                        swap(matrixHolder.SetCoord(l, col), matrixHolder.SetCoord(j, col));
                        swap(inverseHolder.SetCoord(l, col), inverseHolder.SetCoord(j, col));
                    }
                }
                break;
            }
        }
        if (fullZeroColumn) {
            ++k;
            continue;
        }
        for ( int subRow = l + 1; subRow < rows; subRow++ ) {
            nullifyingValue = matrixHolder.GetCoord(subRow, k) / matrixHolder.GetCoord(l, k);
            for (int col = 0; col < cols; col++) {
                matrixHolder.SetCoord(subRow, col)  -= nullifyingValue * matrixHolder.GetCoord(l, col);
                inverseHolder.SetCoord(subRow, col) -= nullifyingValue * inverseHolder.GetCoord(l, col);
            }
        }
        ++k;
        ++l;
    }

    double diagonalValue;

    for ( int row = matrixHolder.GetRows() - 1; row >= 0; row-- ) {
        if (matrixHolder.GetCoord(row, row) == 0) {
            cout << sourceName << " is an irregular matrix and has no inverse." << endl;
            return;
        }
        diagonalValue = matrixHolder.GetCoord(row, row);
        matrixHolder.SetCoord(row, row) = 1.0;
        cout << diagonalValue << endl;
        for ( int col = 0; col < matrixHolder.GetCols(); col++ )
            inverseHolder.SetCoord(row, col) = inverseHolder.GetCoord(row, col) / diagonalValue;
        for ( int upRow = row - 1; upRow >= 0; upRow-- ) {
            nullifyingValue = matrixHolder.GetCoord(upRow, row) / matrixHolder.GetCoord(row, row);
            matrixHolder.SetCoord(upRow, row) -= nullifyingValue * matrixHolder.GetCoord(row, row);
            for (int col = 0; col < matrixHolder.GetCols(); col++)
                inverseHolder.SetCoord(upRow, col) -= nullifyingValue * inverseHolder.GetCoord(row, col);
        }
    }

    try {
        matrixDatabase.AddMatrix(newName, inverseHolder.CreateMatrix());
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to store inversed matrix" << endl;
        throw;
    }

    cout << "Resulting matrix:" << endl;
    matrixDatabase.PrintMatrix(newName);
}