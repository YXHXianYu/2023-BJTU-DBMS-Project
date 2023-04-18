#ifndef __CONSTRAINT_H__
#define __CONSTRAINT_H__

#include <string>

class Constraint {

public:
    Constraint(const std::string& fieldName);

    std::string GetFieldName() const;

    void SetFieldName(const std::string& fieldName);

    virtual void Polymorphic();

private:
    std::string _fieldName;

};

#endif // __CONSTRAINT_H__