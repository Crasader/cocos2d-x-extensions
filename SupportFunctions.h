#ifndef __okeya__SupportFunctions__
#define __okeya__SupportFunctions__

#include <iostream>
#include <vector>
#include <sstream>
#include "cocos/ui/UIImageView.h"

namespace supportfunctions{
    int bit(const int num);
    std::string to_string(int i);

    inline int to_int(const std::string& s) {
        int v;
        std::istringstream sin(s);
        sin >> v;
        return v;
    }

    std::vector<std::string> splitLines(const std::string& str, const int rows);
    std::string strftime(const int timestamp, const char* format);
    cocos2d::ui::ImageView* createImageView(const std::string& filePath);
    const int todayYmd();
    bool isOpening(long int now, long int open, long int close);
}

#endif /* defined(__okeya__SupportFunctions__) */
