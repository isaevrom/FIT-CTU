#include "CCut.hpp"
#include "CMatrixHolder.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <new>
using namespace std;

bool CCut::SetBoundary(int & b, const string & description ) {
    string readLine;
    istringstream iss;
    string emptyString;
    bool metricSet = false;
    while (! metricSet) {
        cout << description;
        if(!getline(cin, readLine))
            return false;
        cout << readLine << endl;
        iss.str(readLine);
        iss.clear();
        if(!(iss >> b)) {
            cout << "Not a valid metric line!" << endl;
            continue;
        }
        if(b < 0) {
            cout << "Metric cannot be less than 0!" << endl;
            continue;
        }
        iss >> emptyString;
        if (!emptyString.empty()) {
            cout << emptyString << endl;
            cout << "Not A valid metric line!" << endl;
            continue;
        }
        metricSet = true;
    }
    return true;
}

bool CCut::ReadContext() {
    if (! SetOperandName(newName, "New matrix name:\t"))
        return false;
    if (! SetOperandName(sourceName, "Cutting matrix:\t"))
        return false;
    if (! SetBoundary(rS, "Starting row: ") )
        return false;
    if (! SetBoundary(rE, "Ending row:") )
        return false;
    if (! SetBoundary(cS, "Starting column: ") )
        return false;
    if (! SetBoundary(cE, "Ending column:") )
        return false;

    return true;
}

void CCut::Execute(CDatabase &matrixDatabase) {
    if (! matrixDatabase.IsPresent(sourceName)) {
        cout << "No matrix called '" << sourceName << "' in the database." << endl;
        cout << "Please, check out list of names of matrices using LIST operation." << endl;
        return;
    }

    if (rS > rE) { // TODO: refactoring
        int tmp;
        tmp = rS;
        rS  = rE;
        rE  = tmp;
    }

    if (cS > cE) {  // TODO: refactoring
        int tmp;
        tmp = cS;
        cS  = cE;
        cE  = tmp;
    }

    int tmpRows = matrixDatabase.RowsNumber(sourceName);
    int tmpCols = matrixDatabase.ColsNumber(sourceName);

    if ( rE >= tmpRows ) {
        cout << "Can't cut beyond matrix row    boundaries!" << endl;
        return;
    }

    if ( cE >= tmpCols ) {
        cout << "Can't cut beyond matrix column boundaries!" << endl;
        return;
    }
    CMatrixHolder matrixHolder(rE - rS + 1, cE - cS + 1);

    for (int row = rS; row <= rE; row++)
        for (int col = cS; col <= cE; col++)
            matrixHolder.SetCoord(row - rS, col - cS)
                 =
            matrixDatabase.AccessMatrix(sourceName)->GetCoord(row, col);
    try {
        matrixDatabase.AddMatrix(newName, matrixHolder.CreateMatrix());
    } catch (const bad_alloc & ba ) {
        cout << "Not enough memory to add CUT matrix." << endl;
        throw;
    }
    cout << "Resulting matrix:" << endl;
    matrixDatabase.PrintMatrix(newName);
}