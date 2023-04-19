#ifndef __COLASQLTOOL_H__
#define __COLASQLTOOL_H__

#include <vector>
#include <string>
#include <any>
#include<cmath>
#include "all_constraints.h"
#include "constants.h"
class ColasqlTool {
public:

    // 输入一个表示类型的string，输出一个该类型的any（值随机）
    static std::any GetAnyByType(const std::string& type);

    // 输入一个表示类型的string 和 表示any数值的string，输出一个该类型的any
    static std::any GetAnyByTypeAndValue(const std::string& type, const std::string& value);

    // 输入一个表示类型的any 和 表示any数值的string，输出一个该类型的any
    static std::any GetAnyByTypeAndValue(const std::any& type, const std::string& value);

    // 输入一个any，以string形式输出该any的值
    static std::string AnyToString(const std::any& any);

    // 用stdout输出fields内的信息
    static void OutputFields(const std::vector<std::pair<std::string, std::any>>& fields);

    //输入两个any,比较它们的大小
    static int CompareAny(const std::any& any1, const std::any& any2);

    // 用stdout输出constraints内的信息
    static void OutputConstraints(const std::vector<Constraint*> constraints);


};

class ColasqlException {

};
class ColasqlNull {

};
#endif // __COLASQLTOOL_H__