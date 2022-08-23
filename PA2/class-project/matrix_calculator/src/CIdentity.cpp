#include "CIdentity.hpp"
#include "CMatrixHolder.hpp"
#include "CDatabase.hpp"
#include <string>
#include <sstream>
#include <new>

using namespace std;

bool CIdentity::ReadContext() {
    if ( !SetOperandName(COperation::newName, "New matrix name:\t" ) )
        return false;
    string readLine;
    string emptyString;
    istringstream iss;
    bool metricSet = false;
    while (! metricSet) {
        cout << "Metric: ";
        if(!getline(cin, readLine))
            return false;
        cout << readLine << endl;
        iss.str(readLine);
        iss.clear();
        if(!(iss >> metric)) {
            cout << "Not valid metric line!" << endl;
            continue;
        }
        if(metric <= 0) {
            cout << "Metric cannot be less than or equal to 0!" << endl;
            continue;
        }

        iss >> emptyString;
        if (!emptyString.empty()) {
            cout << emptyString << endl;
            cout << "Not valid metric line!" << endl;
            continue;
        }
        metricSet = true;
    }

    return true;
}

void CIdentity::Execute(CDatabase &matrixDatabase) {
    CMatrixHolder matrixHolder(metric, metric);
    for (int row = 0; row < metric; row++) {
        for (int col = 0; col < metric; col++)
            if (row == col)
                matrixHolder.SetCoord(row, col) = 1;
    }
    cout << "Resulting matrix:" << endl;
    matrixDatabase.AddMatrix(newName, matrixHolder.CreateMatrix());
}