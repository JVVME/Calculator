//
// Created by zhang on 2025/2/23.
//

#ifndef LOG_H
#define LOG_H
#include <string>
#include <vector>

class M_Log {
public:
    void log_add(const std::vector<std::string> &la);
    void show_log() const;
    void save2file(std::string fname);
private:
    std::vector<std::vector<std::string>> log;
};

#endif //LOG_H
