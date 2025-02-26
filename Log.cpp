//
// Created by zhang on 2025/2/23.
//

#include "include/Log.h"

#include <fstream>
#include <iomanip>
#include <iostream>

void M_Log::log_add(const std::vector<std::string> &la) {
    log.push_back(la);
}

void M_Log::show_log() const {
    int ma = 0, mb = 0, mc = 0;
    for (int i = 0; i < log.size(); i++) {
        ma = std::max(ma, (int)log[i][0].length());
        mb = std::max(mb, (int)log[i][1].length());
        mc = std::max(mc, (int)log[i][2].length());
    }
    std::cout << std::left << std::setw(ma + 5) << "Call";
    std::cout << std::setw(mb + 10) << "Expression";
    std::cout << std::setw(mc + 5) << "Result" <<std::endl;

    for (int i = 0; i < log.size(); i++) {
        std::cout << std::left << std::setw(ma + 5) << log[i][0];
        std::cout << std::setw(mb + 10) << log[i][1];
        std::cout << std::setw(mc + 5) << log[i][2] << std::endl;
    }

}

void M_Log::save2file(std::string fname) {
    std::cout << fname << std::endl;
    std::ofstream outFile;
    outFile.open(fname);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return ;
    }

    int ma = 0, mb = 0, mc = 0;
    for (auto & i : log) {
        ma = std::max(ma, static_cast<int>(i[0].length()));
        mb = std::max(mb, static_cast<int>(i[1].length()));
        mc = std::max(mc, static_cast<int>(i[2].length()));
    }
    outFile << std::left << std::setw(ma + 5) << "Call";
    outFile << std::setw(mb + 10) << "Expression";
    outFile << std::setw(mc + 5) << "Result" <<std::endl;

    for (auto & i : log) {
        outFile << std::left << std::setw(ma + 5) << i[0];
        outFile << std::setw(mb + 10) << i[1];
        outFile << std::setw(mc + 5) << i[2] << std::endl;
    }

    std::cout << "file " << fname << " saved successfully."<<std::endl;

}
