
#include "constraint.h"

Constraint::Constraint(const std::string& fieldName): _fieldName(fieldName) {
}

std::string Constraint::GetFieldName() const {
    return _fieldName;
}

void Constraint::SetFieldName(const std::string& fieldName) {
    _fieldName = fieldName;
}

void Constraint::Polymorphic() {
    // make class Constraint a polymorphic class.
}