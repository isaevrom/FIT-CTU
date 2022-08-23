#include "CMerge.hpp"
#include "CMatrixHolder.hpp"
#include <iostream>
#include <new>

using namespace std;

bool CMerge::ReadContext() {
    if ( !SetOperandName(COperation::newName, "New matrix name:\t" ) )
        return false;
    if ( !SetOperandName(CBinaryOperation::nameA, "Name 1:\t") )
        return false;
    if ( !SetOperandName(CBinaryOperation::nameB, "Name 2:\t") )
        return false;

    while (mergingOperation != "H" && mergingOperation != "V") {
        if(! SetOperandName(mergingOperation, "How (H/V):\t\t") )
            return false;
    }

    return true;
}

void CMerge::Execute(CDatabase & matrixDatabase) {
    if (!matrixDatabase.IsPresent(nameA)) {
        cout << "No matrix called '" << nameA << "' in the database." << endl;
        cout << "Please, check out list of names of matrices using LIST operation." << endl;
        return;
    }
    if (!matrixDatabase.IsPresent(nameB)) {
        cout << "No matrix called '" << nameB << "' in the database." << endl;
        cout << "Please, check out list of names of matrices using LIST operation." << endl;
        return;
    }


    int matrixOneRows = matrixDatabase.RowsNumber(nameA);
    int matrixOneCols = matrixDatabase.ColsNumber(nameA);

    int matrixTwoRows = matrixDatabase.RowsNumber(nameB);
    int matrixTwoCols = matrixDatabase.ColsNumber(nameB);

    CMatrixHolder matrixHolder;

    if ( mergingOperation == "H" ) { // TODO: REFACTORING -- SHORTEN IT
        cout << "Horizontal merging of matrices " << nameA << " and " << nameB << endl;

        if ( matrixOneRows != matrixTwoRows ) {
            cout << "Can't merge horizontally matrices with a different amount of rows!" << endl;
            return;
        }

        matrixHolder.SetRows(matrixOneRows);
        matrixHolder.SetCols(matrixOneCols + matrixTwoCols);
        matrixHolder.Expand();

        for ( int row = 0; row < matrixOneRows; row++ )
            for ( int col = 0; col < matrixOneCols; col++ )
                matrixHolder.SetCoord(row, col)
                    =
                matrixDatabase.AccessMatrix(nameA)->GetCoord(row, col);

        int colOffset = matrixOneCols;

        for ( int row = 0; row < matrixTwoRows; row++ )
            for ( int col = 0; col < matrixTwoCols; col++ )
                matrixHolder.SetCoord(row, col + colOffset)
                    =
                matrixDatabase.AccessMatrix(nameB)->GetCoord(row, col);

    } else if ( mergingOperation == "V" ) {
        cout << "Vertical merging of matrices "   << nameA << " and " << nameB << endl;

        if ( matrixOneCols != matrixTwoCols ) {
            cout << "Can't merge vertically matrices with a different amount of rows!" << endl;
            return;
        }

        matrixHolder.SetRows(matrixOneRows + matrixTwoRows);
        matrixHolder.SetCols(matrixOneCols);
        matrixHolder.Expand();

        for ( int row = 0; row < matrixOneRows; row++ )
            for ( int col = 0; col < matrixOneCols; col++ )
                matrixHolder.SetCoord(row, col)
                        =
                matrixDatabase.AccessMatrix(nameA)->GetCoord(row, col);

        int rowOffset = matrixOneRows;

        for ( int row = 0; row < matrixTwoRows; row++ )
            for ( int col = 0; col < matrixTwoCols; col++ )
                matrixHolder.SetCoord(row + rowOffset, col)
                        =
                matrixDatabase.AccessMatrix(nameB)->GetCoord(row, col);
    }
    try {
        matrixDatabase.AddMatrix(newName, matrixHolder.CreateMatrix());
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to add merged matrix" << endl;
        throw;
    }
    cout << "Resulting matrix:" << endl;
    matrixDatabase.PrintMatrix(newName);
}
