#include <fstream>

namespace logger {
    std::ofstream log_file;
    bool is_debug = false;
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
        log_file << text << std::endl;
    }
}