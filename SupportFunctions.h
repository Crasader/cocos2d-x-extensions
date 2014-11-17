#ifndef __okeya__SupportFunctions__
#define __okeya__SupportFunctions__

#include <iostream>
#include <vector>

namespace supportfunctions{
    int bit(const int num);
    std::string to_string(int i);
    std::vector<std::string> splitLines(const std::string& str, const int rows);
}

#endif /* defined(__okeya__SupportFunctions__) */