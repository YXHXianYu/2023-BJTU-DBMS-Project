#ifndef CONSTANTS_H
#define CONSTANTS_H

const int kSuccess = 0;
const int kUserNameExisted = 1;
const int kUserNameNotFound = 2;
const int kUserPasswordError = 3;
const int kUserNotLogin = 4;
const int kDatabaseNotFound = 5;
const int kDatabaseExisted = 6;
const int kTableNameExisted = 7;
const int kConstraintConflict = 8;


const int kFieldNotFound = 9;
const int kDataTypeWrong = 10;
const int kEqualConditon = 11;
const int kLargerConditon = 12;
const int kLessCondition = 13;
const int kLargerEqualCondition = 14;
const int kLessEqualConditon = 15;
const int kNotEqualConditon =16;

const int kTableNotFound = 17;
const int kConditionsNotSatisfied = 18;
const int kNotSameType = 19;
const int kLarger = 20;
const int kEqual = 21;
const int kLess = 22;

const int kIdentifiedFail = 24;
const int kDatabaseNotUse = 25;
const int kFieldExisted = 25;
const int kErrorCurrentUser = 26;
const int kErrorCurrentDatabase = 27;
const int kSizeNotProper = 28;

const int kInsufficientAuthority = 29;
const int kAuthorityNotFound = 30;
const int kAuthorityExisted = 31;


// ===== About Index =====
const int kUnknownIndex = 100;
const int kFHQTreapIndex = 101;

const int kFailedConditionRelationNotSupport = 110;
const int kFailedConditionDuplicate = 111;
const int kFailedIndexCouldnotSpeedup = 112;
const int kFailedIndexNotBuild = 113;


#endif
