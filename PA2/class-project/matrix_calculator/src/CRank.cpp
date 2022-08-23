#include "CRank.hpp"
#include "CMatrixHolder.hpp"

using namespace std;

bool CRank::ReadContext() {
    if ( !SetOperandName(CUnaryOperation::sourceName, "Source matrix name:\t" ) )
        return false;
    return true;
}

void CRank::Execute(CDatabase &matrixDatabase) {
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
        ++k;
        ++l;
    }

    int rank = 0;

    for (int row = 0; row < matrixHolder.GetRows(); row++)
        for (int col = 0; col < matrixHolder.GetCols(); col++)
            if (matrixHolder.GetCoord(row, col) != 0) {
                rank++;
                break;
            }

    cout << "Rank of " << sourceName << " is " << rank << endl;
}