#ifndef __COMMAND_PROCESSORT_H__
#define __COMMAND_PROCESSORT_H__

#include <iostream>
#include <cassert>
#include <cstdio>
#include <vector>
#include <string>

#include "dataprocessor.h"

#include "my_parser.h"

namespace ColaSQLCommand {

// 命令行处理
class CommandProcessor {
private:
    // Parser
    Parser _parser;

public:
    /**
    * 获取单例
    */
    static CommandProcessor& GetInstance();

    /**
    * Start CLI
    */
    void Start(const std::string& accountName, const std::string& password);

    // 本函数不包含登入！请务必先登入再调用本函数
    std::string Run(std::string input);

    /**
    * ComplexSelect (output result as vector instead of stdout)
    */
    std::string ComplexSelect(std::string input, std::vector<std::vector<std::any>>& result);

    /**
    * Run a script
    */
    std::string RunScript(std::string path);

private:
    /**
    * 输入序列
    */
    std::vector<std::string> seq;

    /**
    * 私有化构造函数
    */
    CommandProcessor();

    /**
    * 返回Prompt()
    */
    std::string GetPrompt();

    /**
    * 序列化
    */
    int Tokenize(std::string input, std::vector<std::string>& result);

    /**
    * 预处理
    */
    int Preprocess(std::string& str);
};

} // ColaSQLCommand

#endif // __COMMAND_PROCESSORT_H__
