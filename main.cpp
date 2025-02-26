#include<iostream>
#include<string>
#include <thread>

#include "include/kernel.h"

void hello_info();

enum SYS_STATE {
    running,
    halt
};

int main() {
    hello_info();

    SYS_STATE state = running;

    while (state != halt) {
        std::string input;
        std::cout << "(cal)>> ";
        std::getline(std::cin, input);
        if (input == "exit") {
            state = halt;
        }
        if (state == running) {
            calculator_kernel(input);
        }
    }

    std::cout << "Program exit success." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}

void hello_info() {
    using namespace std;
    cout << "This is a calculator program written by Marco. You can perform many operations in the console.\n"
            "   If you are not very familiar with it, you can use the help command to view the documentation, \n"
            "which will be saved in the current directory.\n";
    cout << "Calculator running:" << endl;
}
