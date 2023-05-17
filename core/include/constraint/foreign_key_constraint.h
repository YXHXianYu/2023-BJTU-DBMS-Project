#ifndef __FOREIGN_KEY_CONSTRAINT_H__
#define __FOREIGN_KEY_CONSTRAINT_H__

#include <string>
#include <any>

#include "constraint.h"

// 主键：非空 且 唯一
class ForeignKeyConstraint: public Constraint {

public:
    ForeignKeyConstraint(const std::string& fieldName,
                         const std::string& constraintName, 
                         const std::string& referenceTableName,
                         const std::string& referenceFieldName);

    std::string GetReferenceTableName() const;

    void SetReferenceTableName(const std::string& referenceTableName);
    
    std::string GetReferenceFieldName() const;

    void SetReferenceFieldName(const std::string& referenceFieldName);

private:
    std::string _referenceTableName;
    std::string _referenceFieldName;

};

#endif // __FOREIGN_KEY_CONSTRAINT_H__
