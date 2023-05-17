#ifndef __CONSTRAINT_H__
#define __CONSTRAINT_H__

#include <string>

class Constraint {

public:
    Constraint(const std::string& fieldName, const std::string& constraintName);

    std::string GetFieldName() const;

    void SetFieldName(const std::string& fieldName);

    std::string GetConstraintName() const;

    void SetConstraintName(const std::string& constraintName);

    virtual void Polymorphic();

private:
    std::string _fieldName;

    std::string _constraintName;

};

#endif // __CONSTRAINT_H__
