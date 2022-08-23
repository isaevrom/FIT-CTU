#include "IMatrix.hpp"

IMatrix::IMatrix(int rows, int cols) : rows(rows), cols(cols) {}

int IMatrix::GetRows() const { return rows; }
int IMatrix::GetCols() const { return cols; }