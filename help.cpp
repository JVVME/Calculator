//
// Created by zhang on 2025/2/23.
//

#include "include/help.h"

#include <iostream>
#include <fstream>

void document(std::string language) {
    if (language == "zh-cn") {
        std::cout << "Check the document in $(path_to_project)/doc" << std::endl;
    }
}
