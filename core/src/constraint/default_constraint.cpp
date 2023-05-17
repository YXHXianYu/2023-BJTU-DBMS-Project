#include "default_constraint.h"

DefaultConstraint::DefaultConstraint(const std::string& fieldName, const std::string& constraintName, std::any value):
    Constraint(fieldName, constraintName), _value(value) {
}

std::any DefaultConstraint::GetValue() const {
    return _value;
}

void DefaultConstraint::SetValue(std::any value) {
    _value = value;
}
