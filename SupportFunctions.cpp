#include "SupportFunctions.h"
#include <sstream>
#include <iomanip>
#include "cocos2d.h"

using namespace cocos2d;
using namespace cocos2d::ui;

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
        return lines;
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
    bool isOpening(long int now, long int open, long int close)
    {
        if((open == 0 || open <= now) && (close == 0 || now <= close)){
            return true;
        }else{
            return false;
        }
    }

    const int todayYmd()
    {
        auto now = std::chrono::system_clock::now();
        time_t in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d");
        return to_int(ss.str());
    }
    
    cocos2d::ui::ImageView* createImageView(const std::string& filePath)
    {
        ImageView::TextureResType resType;
        if(SpriteFrameCache::getInstance()->getSpriteFrameByName(filePath)){
            resType = ImageView::TextureResType::PLIST;
        }else{
            resType = ImageView::TextureResType::LOCAL;
        }
        return ImageView::create(filePath, resType);
    }
}
