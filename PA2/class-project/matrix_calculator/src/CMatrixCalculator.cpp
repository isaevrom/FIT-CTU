#include "CMatrixCalculator.hpp"
#include "COperation.hpp"
#include "CAddition.hpp"
#include "CSubstraction.hpp"
#include "CMultiplication.hpp"
#include "CScan.hpp"
#include "CTranspose.hpp"
#include "CMerge.hpp"
#include "CIdentity.hpp"
#include "CCut.hpp"
#include "CGEM.hpp"
#include "CDeterminant.hpp"
#include "CRank.hpp"
#include "CInverse.hpp"
#include <iostream>
#include <memory>
#include <exception>
#include <new>

using namespace std;

const set<string> CMatrixCalculator::opList{
        "PRINT", "LIST",
        "QUIT", "SAVE", "LOAD",
        "SCAN", "IDENTITY",
        "+", "-", "*", "MERGE", "CUT",
        "GEM", "TRANSPOSE", "DET", "RANK"
};

const set<char> CMatrixCalculator::opSymbols {
    '+', '-', '*'
};

void CMatrixCalculator::ListOperations() {
    cout << "Operations list:" << endl;
    cout << "#------------------------------" << endl;
    for (const auto & op: opList)
        cout << op << endl;
    cout << "------------------------------#" << endl;
}

bool CMatrixCalculator::ReadOperation(string & opName) {
    bool isOpNameSet = false;
    while (! isOpNameSet ) {
        cout << "\nOperation:\t\t";
        isOpNameSet = true;
        if ( ! getline(cin, opName) )
            return false;
        cout << "------------------------------" << endl;
        for (char i : opName) {
            if (!isalpha(i) && opSymbols.count(i) == 0) {
                cout << "Please, enter only { a-z, A-Z, +, -, * } symbols." << endl;
                isOpNameSet = false;
                break;
            }
        }
        if (opName.empty()) {
            cout << "Please, enter only { a-z, A-Z, +, -, * } symbols." << endl;
            isOpNameSet = false;
        }
    }
    return true;
}

bool CMatrixCalculator::ReadName(string & name) {
    bool isNameSet = false;
    while (! isNameSet ) {
        cout << "Name:\t\t";
        isNameSet = true;
        if ( ! getline(cin, name) )
            return false;
        for (char i : name) {
            if (!isalpha(i)) {
                cout << "Please, enter only { a-z, A-Z } symbols." << endl;
                isNameSet = false;
                break;
            }
        }
        if (name.empty()) {
            cout << "Please, enter only { a-z, A-Z } symbols." << endl;
            isNameSet = false;
        }
    }
    return true;
}

shared_ptr<COperation> CMatrixCalculator::LoadOperation(const string & opName) {
    try {
        if (opName == "+")
            return make_shared<CAddition>();
        else if (opName == "-")
            return make_shared<CSubstraction>();
        else if (opName == "*")
            return make_shared<CMultiplication>();
        else if (opName == "TRANSPOSE")
            return make_shared<CTranspose>();
        else if (opName == "MERGE")
            return make_shared<CMerge>();
        else if (opName == "CUT")
            return make_shared<CCut>();
        else if (opName == "SCAN")
            return make_shared<CScan>();
        else if (opName == "IDENTITY")
            return make_shared<CIdentity>();
        else if (opName == "GEM")
            return make_shared<CGEM>();
        else if (opName == "DET")
            return make_shared<CDeterminant>();
        else if (opName == "RANK")
            return make_shared<CRank>();
        else if (opName == "INVERSE")
            return make_shared<CInverse>();
        // for unknown operation ret nullptr
    } catch (const bad_alloc & ba) {
        cout << "Not enough memory to create a new operation." << endl;
        throw;
    }
    return nullptr;
}

void CMatrixCalculator::Run() {
    cout << "#######################################" << endl;
    cout << "#          Welcome, dear user!        #" << endl;
    cout << "#######################################" << endl;
    cout << "# type 'op' and hit enter to see info #" << endl;
    cout << "#######################################" << endl;


    
    shared_ptr<COperation> operation;
    string matrixName;
    string currentOpName;
    while (true) {
        if (!ReadOperation(currentOpName))
            return;


        if (currentOpName == "PRINT") {
            if ( ! ReadName(matrixName) )
                return;
            matrixDatabase.PrintMatrix(matrixName);

        } else if (currentOpName == "LIST") {
            matrixDatabase.ListMatrices();

        } else if (currentOpName == "SAVE") {
            if (!ReadName(matrixName))
                return;
            if ( matrixDatabase.IsPresent(matrixName) ) {
                if (!matrixDatabase.AccessMatrix(matrixName)->SaveMatrix(matrixName)) {
                    cout << "Running this program is no longer safe" << endl;
                    cout << "as there are some issues with your device's disk." << endl;
                    return;
                }
                cout << "Matrix " << matrixName << " successfully saved!" << endl;
            } else {
                cout << "There is no matrix called '" << matrixName << "' in the database." << endl;
            }

        } else if (currentOpName == "LOAD") {
            cout << "TODO: Load matrix" << endl;
            if (!ReadName(matrixName))
                return;
            string saveAsName;
            cout << "Storing ";
            if (!ReadName(saveAsName))
                return;
            if ( matrixDatabase.LoadMatrix(matrixName, saveAsName) )
                cout << "Matrix successfully loaded!" << endl;
        } else if (currentOpName == "DELETE") {
            if (!ReadName(matrixName))
                return;
            matrixDatabase.DeleteMatrix(matrixName);

        } else if (currentOpName == "QUIT") {
            cout << "See you, dear user!" << endl;
            break;

        } else if (currentOpName == "op") {
            ListOperations();
            continue;

        } else {
            try {
                operation = LoadOperation(currentOpName);
                if (operation == nullptr) {
                    cout << "Consider running:" << endl;
                    cout << "\t\t'op' operation to see existing operations" << endl;
                    continue;
                }
                try {
                    if (!operation->ReadContext())
                        return;
                    try {
                        operation->Execute(matrixDatabase);
                    } catch (const out_of_range &oor) {
                        cout << oor.what();
                        return;
                    }
                } catch (const invalid_argument & ia) {
                    cout << ia.what() << endl;
                    return;
                }
            } catch (const bad_alloc &ba) {
                cout << ba.what();
                cout << "Ran out of heap memory!" << endl;
                return;
            }
        }
    }
}
