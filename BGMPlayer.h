#ifndef __okeya__BGMPlayer__
#define __okeya__BGMPlayer__

#include "audio/include/AudioEngine.h"
#include "cocos2d.h"

using namespace cocos2d;

class BGMPlayer
{
public:
    static void play(const std::string& path);
private:
    static std::string _currentMusic;
    static int _currentAudioId;
};

#endif /* defined(__okeya__BGMPlayer__) */
