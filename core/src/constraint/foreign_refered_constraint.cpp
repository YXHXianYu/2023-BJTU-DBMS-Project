#include "foreign_refered_constraint.h"

ForeignReferedConstraint::ForeignReferedConstraint(const std::string& fieldName,
                                                   const std::string& constraintName,
                                                   const std::string& referenceTableName,
                                                   const std::string& referenceFieldName):
    Constraint(fieldName, constraintName), _referenceTableName(referenceTableName), _referenceFieldName(referenceFieldName) {
}

std::string ForeignReferedConstraint::GetReferenceTableName() const {
    return _referenceTableName;
}

void ForeignReferedConstraint::SetReferenceTableName(const std::string& referenceTableName) {
    _referenceTableName = referenceTableName;
}

std::string ForeignReferedConstraint::GetReferenceFieldName() const {
    return _referenceFieldName;
}

void ForeignReferedConstraint::SetReferenceFieldName(const std::string& referenceFieldName) {
    _referenceFieldName = referenceFieldName;
}
