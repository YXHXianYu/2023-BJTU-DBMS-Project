#ifndef __UNIQUE_CONSTRAINT_H__
#define __UNIQUE_CONSTRAINT_H__

#include <string>

#include "constraint.h"

// 非空
class UniqueConstraint: public Constraint {
public:
    UniqueConstraint(const std::string& fieldName, const std::string& constraintName);
};

#endif // __UNIQUE_CONSTRAINT_H__
