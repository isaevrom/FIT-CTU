#include "CMatrixHolder.hpp"
#include "CSparseMatrix.hpp"
#include "CDenseMatrix.hpp"
#include "IMatrix.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <iomanip>

using namespace std;

CMatrixHolder::CMatrixHolder(int rows, int cols) : rows(rows), cols(cols) {
    Expand();
}

void CMatrixHolder::Expand() {
    vector<double> oneRowOfMatrix;
    if (rows == 0 || cols == 0)
        throw invalid_argument("CMatrixHolder::Expand -- Trying to expand matrix holder with 0 rows/cols.");
    for ( int i = 0; i < cols; i++ )
        oneRowOfMatrix.push_back(0);
    for ( int i = 0; i < rows; i++ )
        this->matrixHolder.push_back(oneRowOfMatrix);
}

double   CMatrixHolder::GetCoord(int row, int col) const {
    if (row >= rows || col >= cols)
        throw out_of_range("CMatrixHolder::GetCoord -- Trying to read past matrix holder data range!");
    return matrixHolder[row][col];
}
double & CMatrixHolder::SetCoord(int row, int col) {
    if (row >= rows || col >= cols)
        throw out_of_range("CMatrixHolder::SetCoord -- Trying to read past matrix holder data range!");
    return matrixHolder[row][col];
}

int CMatrixHolder::GetRows() const { return rows; }
int CMatrixHolder::GetCols() const { return cols; }

void CMatrixHolder::SetRows(int rows) { this->rows = rows; }

void CMatrixHolder::SetCols(int cols) { this->cols = cols; }

int CMatrixHolder::GetDensity() const {
    int density = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (matrixHolder[row][col] != 0) // TODO: DBL_EPSILON
                ++density;
        }
    }
    return density;
}

shared_ptr<IMatrix> CMatrixHolder::CreateMatrix() const {
    try {
        if (GetDensity() > rows * cols / 2) { // MORE THAN 50% OF ELEMENTS ARE NON-ZERO => DENSE
            cout << "Dense Matrix. Density = " << GetDensity() << endl;
            return make_shared<CDenseMatrix>(CDenseMatrix(matrixHolder, rows, cols));
        }
        cout << "Sparse Matrix.  Density = " << GetDensity() << endl;
        return make_shared<CSparseMatrix>(CSparseMatrix(matrixHolder, rows, cols));
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to instantiate a new matrix." << endl;
        throw;
    }
}

void CMatrixHolder::Print() const {
    cout << "CMatrixHolder:" << endl;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++)
            cout << fixed << setprecision(3) << matrixHolder[row][col] << " ";
        cout << endl;
    }
}