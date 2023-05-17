#include "primary_key_constraint.h"

PrimaryKeyConstraint::PrimaryKeyConstraint(const std::string& fieldName, const std::string& constraintName):
    Constraint(fieldName, constraintName) {
    
}
