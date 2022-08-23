#include "CBinaryOperation.hpp"
#include <string>

using namespace std;

bool CBinaryOperation::ReadContext() {
    if ( !SetOperandName(COperation::newName, "New matrix name:\t" ) )
        return false;
    if ( !SetOperandName(CBinaryOperation::nameA, "Name 1:\t") )
        return false;
    if ( !SetOperandName(CBinaryOperation::nameB, "Name 2:\t") )
        return false;
    return true;
}