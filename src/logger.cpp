#include "header.hpp"
#include <fstream>

namespace logger {
    std::ofstream log_file;
    bool is_debug = true;
    void init()
    {
        if (is_debug)
        {
            log_file.open("debug.log");
        }
    }
    void print_log(const std::string& text)
    {
        if (!is_debug)
        {
            return;
        }
        log_file << "[" + std::to_string(get_frame_count()) + "] " << text << std::endl;
    }
}