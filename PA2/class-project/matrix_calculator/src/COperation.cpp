#include "COperation.hpp"
#include <iostream>
#include <string>

using namespace std;

bool COperation::SetOperandName(string & operand, const string & message) {
    bool isSet = false;
    while ( !isSet ) {
        isSet = true;
        cout << message;
        if( ! getline(cin, operand) )
            return false;
        for (char i : operand) {
            if (!isalpha(i)) {
                cout << "Please, enter only { a-z, A-Z } symbols." << endl;
                isSet = false;
                break;
            }
        }
        if (!isSet)
            continue;
        if (operand.empty()) {
            cout << "Please, enter only { a-z, A-Z } symbols." << endl;
            isSet = false;
        }
    }
    return true;
}