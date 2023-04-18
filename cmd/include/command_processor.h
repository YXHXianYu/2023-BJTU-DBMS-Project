#ifndef __COMMAND_PROCESSORT_H__
#define __COMMAND_PROCESSORT_H__

#include <iostream>
#include <cassert>
#include <cstdio>
#include <vector>
#include <string>

#include "my_parser.h"

namespace ColaSQLCommand {

// 命令行处理
class CommandProcessor {
private:
    Parser _parser;

public:
    static CommandProcessor GetInstance();
    void Start();

private:
    CommandProcessor();
    std::string GetPrompt();
    int Tokenize(std::string input, std::vector<std::string>& result);
    int ToLowerCase(std::string& str);
};

} // ColaSQLCommand

#endif // __COMMAND_PROCESSORT_H__