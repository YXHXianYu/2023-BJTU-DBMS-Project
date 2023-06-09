/**
 * author: drj
 * 工具类。
 * 含有对any类型进行操作的接口、输出表信息的接口、数据结构转换的接口、对其它数据结构转换成表结构的接口等等。
*/
#ifndef __COLASQLTOOL_H__
#define __COLASQLTOOL_H__

#include <vector>
#include <string>
#include <any>
#include <cmath>

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
    static void OutputFields(const std::vector<std::pair<std::string, std::string>>& fields);

    //输入两个any,比较它们的大小
    static int CompareAny(const std::any& any1, const std::any& any2);

    // 用stdout输出constraints内的信息
    static void OutputConstraints(const std::vector<Constraint*> constraints);

    // 输出选择结果
    static std::string OutputSelectResult(const std::vector<std::vector<std::any>> result);

    //把string 转换成表结构, head为表头, 默认为空
    static std::vector<std::vector<std::any>> ChangeStringsToRecords(std::vector<std::string> strings, std::string head);

    //把表描述信息转换成表结构
    static std::vector<std::vector<std::any>> ChangeDescriptionToRecords(const std::vector<std::pair<std::string, std::string>>& fields,
                                                                         const std::vector<Constraint*>&constraints);

    // 把约束条件vector转换成字符串（用于文件输出）
    static std::string ConstraintsToString(const std::vector<Constraint*>& constraints);

    // 把字符串转换成约束条件vector
    // static std::vector<Constraint*> StringToConstraints(const std::string& str);
};

class ColasqlException {

};
class ColasqlNull {

};
#endif // __COLASQLTOOL_H__
