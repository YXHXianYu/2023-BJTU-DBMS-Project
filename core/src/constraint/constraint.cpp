
#include "constraint.h"

Constraint::Constraint(const std::string& fieldName, const std::string& constraintName):
    _fieldName(fieldName), _constraintName(constraintName) {
}

std::string Constraint::GetFieldName() const {
    return _fieldName;
}

void Constraint::SetFieldName(const std::string& fieldName) {
    _fieldName = fieldName;
}

std::string Constraint::GetConstraintName() const {
    return _constraintName;
}

void Constraint::SetConstraintName(const std::string& constraintName) {
    _constraintName = constraintName;
}

void Constraint::Polymorphic() {
    // make class Constraint a polymorphic class.
}
