#include <iostream>

#include "command_processor.h"

int main(int argc, char** argv) {

    // TODO 调用 ColaSQLCommand
    ColaSQLCommand::CommandProcessor::GetInstance().Start();

    return 0;
}