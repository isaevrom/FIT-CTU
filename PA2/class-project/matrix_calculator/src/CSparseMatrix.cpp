#include "CSparseMatrix.hpp"
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

CSparseMatrix::CSparseMatrix(const vector<vector<double>> & vec_matrix,
                             int rows, int cols)
        : IMatrix(rows, cols)
{
    for (int row = 0; row < IMatrix::rows; row++) {
        for (int col = 0; col < IMatrix::cols; col++) {
            if (vec_matrix[row][col] != 0) // TODO: DBL_EPSILON
                matrix[{row, col}] = vec_matrix[row][col];
        }
    }
}

double CSparseMatrix::GetCoord(int row, int col) const {
    if ( ( row >= IMatrix::rows || row < 0 )
         ||
         ( col >= IMatrix::cols || col < 0 ) )
        throw out_of_range(" CSparseMatrix::GetCoord -- Trying to read past data range!");
    if ( matrix.count({row, col}) > 0 )
        return matrix.at({row, col});
    return 0;
}

bool CSparseMatrix::SaveMatrix(const std::string & name) const {
    string fileName = "matrix_" + name;
    ofstream outFile( fileName );
    if ( !outFile ) { cout << "Output file could not be created." << endl; return false; }
    if ( ! (outFile << "sparse" << " " << rows << " " << cols ) )
        return false;
    if ( ! (outFile << "\n" << matrix.size() ) )
        return false;
    for (const auto &coord : matrix) {
        if (!(outFile << "\n"))
            return false;
        if (!(outFile << " " << coord.first.first << " " << coord.first.second << " " << coord.second))
            return false;
    }
    outFile.close();
    return true;
}

void CSparseMatrix::Print() const {
    cout << "Overridden Print in CSparseMatrix!" << endl;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (matrix.count({row, col}) > 0)
                cout << fixed << setprecision(3)  << matrix.at({row, col}) << " ";
            else
                cout << fixed << setprecision(3)  << 0.0 << " ";
        }
        cout << endl;
    }
}