#include <iostream>
#include"colasqltool.h"
#include"constants.h"
int main() {

    std::cout << "rua" << std::endl;
    std::any a1 = (float)6.888;
    std::any a2 = (float)6.888;
    int comp = ColasqlTool::CompareAny(a1, a2);
    if(comp == kLess) {
        std::cout<<"Less"<<std::endl;
    }
    if(comp == kLarger) {
        std::cout<<"Larger"<<std::endl;
    }
    if(comp == kEqual) {
        std::cout<<"Equal"<<std::endl;
    }
    return 0;
}