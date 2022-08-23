#include "CUnaryOperation.hpp"
#include <string>

bool CUnaryOperation::ReadContext() {
    if ( !SetOperandName(COperation::newName, "New matrix name:\t" ) )
        return false;
    if ( !SetOperandName(sourceName, "Source matrix name:\t" ) )
        return false;

    return true;
}