#include "command_processor.h"

namespace ColaSQLCommand {

CommandProcessor CommandProcessor::GetInstance() {
    static CommandProcessor instance;
    return instance;
}

void CommandProcessor::Start(const std::string& accountName, const std::string& password) {

    // TODO: Login
    DataProcessor::GetInstance().CreateUser(accountName, password);
    DataProcessor::GetInstance().Login(accountName, password);

    // TODO: Welcome information

    std::vector<std::string> seq;
    int ret;

    while(true) { // 主循环
        std::cout << GetPrompt();

        // input
        std::string input;
        std::getline(std::cin, input);

        // quit
        if(input.length() >= 4 && input.substr(0, 4) == "quit") {
            break;
        }

        // preprocess
        Preprocess(input);

        // tokenize
        ret = Tokenize(input, seq);
        
        if(ret == -1) { // 语句异常（语句中包含不正常的分号）
            std::cout << "Error: Statement error!" << std::endl;
            seq.clear();
            continue;
        }
        
        if(ret == 1) { // 语句未结束
            continue;
        }

        // parse
        std::string output;
        output = _parser.Parse(seq);
        std::cout << output << std::endl;
        seq.clear();
    }
}

CommandProcessor::CommandProcessor() {}

std::string CommandProcessor::GetPrompt() {
    return "Co1aSQL > ";
}

// 将input追加进result
// 注意，本函数不会清空result，只会在后面追加！
// 若返回 0，则说明语句已结束
// 若返回 1，则说明语句未结束
// 若返回 -1，则说明语句异常（语句中包含不正常的分号）
int CommandProcessor::Tokenize(std::string input, std::vector<std::string>& result) {

    bool haveEnd = false; // 是否以分号结尾
    if(input[input.length() - 1] == ';') {
        haveEnd = true;
        input = input.substr(0, input.length() - 1); // 删去分号
    }

    input = input + " ";

    for(int st = 0, i = 0; i < input.length(); i++) {
        if(input[i] == ';') return -1; // ERROR
        if(input[i] != ' ' && input[i] != '\t') continue;

        if(i - st >= 1) result.push_back(input.substr(st, i - st));
        st = i + 1;
    }

    return haveEnd ? 0 : 1;
}


int CommandProcessor::Preprocess(std::string& str) {
    if(str.empty()) return -1;

    int del = 'a'-'A';
    for(int i = 0; i < str.length(); i++) {
        if('A' <= str[i] && str[i] <= 'Z') { // 全小写
            str[i] += del;
        } else if(str[i] == ',' || str[i] == '(' || str[i] == ')') { // 替换字符
            str[i] = ' ';
        }
    }

    // 删去前后多余空格 (trim)
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);

    return 0;
}


} // ColaSQLCommand