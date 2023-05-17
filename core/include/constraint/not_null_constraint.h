#ifndef __NOT_NULL_CONSTRAINT_H__
#define __NOT_NULL_CONSTRAINT_H__

#include <string>

#include "constraint.h"

// 非空
class NotNullConstraint: public Constraint {
public:
    NotNullConstraint(const std::string& fieldName, const std::string& constraintName);
};

#endif // __NOT_NULL_CONSTRAINT_H__
