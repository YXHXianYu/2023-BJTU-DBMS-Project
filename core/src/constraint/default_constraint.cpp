#include "default_constraint.h"

DefaultConstraint::DefaultConstraint(const std::string& fieldName, std::any value): Constraint(fieldName), _value(value) {
}

std::any DefaultConstraint::GetValue() const {
    return _value;
}

void DefaultConstraint::SetValue(std::any value) {
    _value = value;
}