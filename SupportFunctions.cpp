#include "SupportFunctions.h"
#include <sstream>

namespace supportfunctions {
    int bit(const int num){
        return ((unsigned int)1 << (num));
    }
    std::string to_string(int i){
        std::stringstream ss;
        ss << i;
        return ss.str();
    }
    
    std::vector<std::string> splitLines(const std::string& str, const int rows)
    {
        std::vector<std::string> lines;
        std::string chunk = "";
        size_t current = 0, found;
        auto delim = "\n";
        int n = 0;
        while((found = str.find_first_of(delim, current)) != std::string::npos){
            n++;
            if(n >= rows){
                chunk += str.substr(current, found - current);
                lines.push_back(chunk);
                chunk = std::string("");
                n = 0;
            }else{
                chunk += str.substr(current, found - current + 1);
            }
            current = found + 1;
        }
        chunk += std::string(str, current, str.size() - current);
        lines.push_back(chunk);
        return std::move(lines);
    }

    std::string strftime(const int timestamp, const char* format)
    {
        time_t time = timestamp;
        struct tm * ptm = localtime(&time);
        size_t length = strlen(format) * 4;
        char buf[length];
        strftime(buf, length, format,  ptm);
        return buf;
    }
}
