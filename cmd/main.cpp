#include <iostream>
#include <string>

#include "command_processor.h"

int main(int argc, char** argv) {

    if(argc <= 2) {
        std::cout << "Please enter account name and password after \"colasql\"" << std::endl;
        return 0;
    }

    ColaSQLCommand::CommandProcessor::GetInstance().Start(std::string(argv[1]), std::string(argv[2]));

    return 0;
}