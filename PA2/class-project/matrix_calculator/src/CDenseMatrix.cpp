#include "CDenseMatrix.hpp"
#include "IMatrix.hpp"
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <fstream>

using namespace std;

CDenseMatrix::CDenseMatrix(const vector<vector<double>> &vec_matrix,
                           int rows, int cols)
        : IMatrix::IMatrix(rows, cols),
          matrix(vec_matrix)
{
}

double CDenseMatrix::GetCoord(int row, int col) const {
    if ( ( row >= IMatrix::rows || row < 0 )
         ||
         ( col >= IMatrix::cols || col < 0 ) ) {
        cout << "row = " << row << " col = " << col << endl;
        throw out_of_range("CDenseMatrix::GetCoord -- Trying to read past data range!");
    }
    return matrix[row][col];
}

bool CDenseMatrix::SaveMatrix(const std::string & name) const {
    string fileName = "matrix_" + name;
    ofstream outFile( fileName );
    if ( !outFile ) { cout << "File " << fileName << " could not be opened." << endl; return true; }

    if ( ! (outFile << "dense" << " " << rows << " " << cols ) )
        return false;

    for (int row = 0; row < rows; row++) {
        if ( ! (outFile << "\n" ) )
            return false;
        for (int col = 0; col < cols; col++) {
            if ( ! (outFile << " " << matrix[row][col]) )
                return false;
        }
    }
    outFile.close();
    return true;
}

void CDenseMatrix::Print() const {
    cout << "Overridden Print in CDenseMatrix!" << endl;
    for (int row = 0; row < IMatrix::rows; row++) {
        for (int col = 0; col < IMatrix::cols; col++)
            cout << fixed << setprecision(3) << matrix[row][col] << " ";
        cout << endl;
    }
}