#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>

namespace FastPlay
{
    extern int start_mode;
    extern int start_slot;
    extern std::vector<std::string> save_slots;

    void Init(const char* mode, const char* slots);
}
