#include "CScan.hpp"
#include "CMatrixHolder.hpp"
#include <iostream>
#include <sstream>
#include <new>

using namespace std;

bool CScan::ReadRows() {
    string readLine;
    istringstream iss;
    string emptyString;

    bool rowRead = false;
    for (int row = 0; row < rows; row++) {
        rowRead = true;
        emptyString = "";
        cout << "row " << row << ": ";
        if ( ! getline(cin, readLine) )
            return false;
        cout << readLine << endl;
        iss.str(readLine);
        iss.clear();
        for ( int col = 0; col < cols; col++ ) {
            if ( ! ( iss >> matrixHolder.SetCoord(row, col) ) ) {
                cout << "Not a valid row! Try again, please." << endl;
                --row;
                rowRead = false;
                break;
            }
        }
        if (! rowRead)
            continue;
        iss >> emptyString;
        if (!emptyString.empty()) {
            cout << "Not a valid row! Try again, please." << endl;
            --row;
        }
    }
    return true;
}

bool CScan::ReadContext() {
    if ( !SetOperandName(COperation::newName, "New matrix name:\t" ) )
        return false;

    string readLine;

    bool metricsSet = false;
    istringstream iss;
    string emptyString;

    while (! metricsSet) {
        cout << "Rows & cols: ";
        if(!getline(cin, readLine))
            return false;
        cout << readLine << endl;
        iss.str(readLine);
        iss.clear();
        emptyString = "";
        if(!(iss >> rows)) {
            cout << "Not valid metrics line!" << endl;
            continue;
        }
        if(rows <= 0) {
            cout << "Metric cannot be less than or equal to 0!" << endl;
            continue;
        }
        if(!(iss >> cols)) {
            cout << "Not valid metrics line!" << endl;
            continue;
        }
        if(cols <= 0) {
            cout << "Metric cannot be less than or equal to 0!" << endl;
            continue;
        }
        iss >> emptyString;
        if (!emptyString.empty()) {
            cout << "emptyString: " << emptyString << endl;
            cout << "Not valid metrics line!" << endl;
            continue;
        }
        metricsSet = true;
    }

    this->matrixHolder.SetRows(rows);
    this->matrixHolder.SetCols(cols);
    this->matrixHolder.Expand();
    if (!ReadRows())
        return false;
    matrixHolder.Print();
    return true;
}

void CScan::Execute(CDatabase &matrixDatabase) {
    try {
        matrixDatabase.AddMatrix(newName, this->matrixHolder.CreateMatrix());
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to add scanned matrix" << endl;
        throw;
    }
}