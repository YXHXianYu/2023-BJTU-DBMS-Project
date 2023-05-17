#include "foreign_key_constraint.h"

ForeignKeyConstraint::ForeignKeyConstraint(const std::string& fieldName,
                                           const std::string& constraintName,
                                           const std::string& referenceTableName,
                                           const std::string& referenceFieldName):
    Constraint(fieldName, constraintName), _referenceTableName(referenceTableName), _referenceFieldName(referenceFieldName) {
}

std::string ForeignKeyConstraint::GetReferenceTableName() const {
    return _referenceTableName;
}

void ForeignKeyConstraint::SetReferenceTableName(const std::string& referenceTableName) {
    _referenceTableName = referenceTableName;
}

std::string ForeignKeyConstraint::GetReferenceFieldName() const {
    return _referenceFieldName;
}

void ForeignKeyConstraint::SetReferenceFieldName(const std::string& referenceFieldName) {
    _referenceFieldName = referenceFieldName;
}
