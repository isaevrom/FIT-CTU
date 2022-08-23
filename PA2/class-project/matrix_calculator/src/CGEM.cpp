#include "CGEM.hpp"
#include <iostream>
#include "CMatrixHolder.hpp"
#include <new>

using namespace std;

bool CGEM::ReadDetails() {
    string detail;
    while (detail != "Y" && detail != "N") {
        if(! SetOperandName(detail, "Detailed (Y/N):\t\t") )
            return false;
    }
    if (detail == "Y")
        isDetailed = true;
    else
        isDetailed = false;
    return true;
}

bool CGEM::ReadContext() {
    if ( !SetOperandName(COperation::newName, "New matrix name:\t" ) )
        return false;
    if ( !SetOperandName(sourceName, "Source matrix name:\t" ) )
        return false;
    if ( !ReadDetails() )
        return false;
    return true;
}

void CGEM::Execute(CDatabase &matrixDatabase) {
    if (! matrixDatabase.IsPresent(sourceName)) {
        cout << "No matrix called '" << sourceName << "' in the database." << endl;
        cout << "Please, check out list of names of matrices using LIST operation." << endl;
        return;
    }

    CMatrixHolder matrixHolder( matrixDatabase.RowsNumber(sourceName),
                                matrixDatabase.ColsNumber(sourceName));
    for (int row = 0; row < matrixHolder.GetRows(); row++)
        for (int col = 0; col < matrixHolder.GetCols(); col++)
            matrixHolder.SetCoord(row, col) = matrixDatabase.AccessMatrix(sourceName)->GetCoord(row, col);

    if (isDetailed) {
        cout << "Before GEM:" << endl;
        matrixHolder.Print();
    }

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
                    }
                }
                if (isDetailed)
                    matrixHolder.Print();
                break;
            }
        }
        if (fullZeroColumn) {
            ++k;
            continue;
        }
        for ( int subRow = l + 1; subRow < rows; subRow++ ) {
            nullifyingValue = matrixHolder.GetCoord(subRow, k) / matrixHolder.GetCoord(l, k);
            for (int col = 0; col < cols; col++)
                matrixHolder.SetCoord(subRow, col) -= nullifyingValue * matrixHolder.GetCoord(l, col);
        }
        if (isDetailed)
            matrixHolder.Print();
        ++k;
        ++l;
    }
    try {
        matrixDatabase.AddMatrix(newName, matrixHolder.CreateMatrix());
    } catch (const bad_alloc & ba ) {
        cout << "Not enough memory to add GEMmed matrix." << endl;
        throw;
    }
    cout << "Resulting matrix:" << endl;
    matrixDatabase.PrintMatrix(newName);
}
