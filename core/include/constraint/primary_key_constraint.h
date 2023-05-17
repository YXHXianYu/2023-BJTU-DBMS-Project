#ifndef __PRIMARY_KEY_CONSTRAINT_H__
#define __PRIMARY_KEY_CONSTRAINT_H__

#include <string>

#include "constraint.h"

// 主键：非空 且 唯一
class PrimaryKeyConstraint: public Constraint {
public:
    PrimaryKeyConstraint(const std::string& fieldName, const std::string& constraintName);
};

#endif // __PRIMARY_KEY_CONSTRAINT_H__
