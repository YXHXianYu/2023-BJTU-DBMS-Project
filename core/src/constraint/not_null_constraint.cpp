#include "not_null_constraint.h"

NotNullConstraint::NotNullConstraint(const std::string& fieldName,
                                     const std::string& constraintName):
    Constraint(fieldName, constraintName) {
    
}
