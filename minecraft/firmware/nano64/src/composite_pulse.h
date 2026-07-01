#pragma once
#include <string>

class CompositePulse {
public:
    explicit CompositePulse(const std::string &path);
    bool load();

private:
    std::string pulsePath;
};
