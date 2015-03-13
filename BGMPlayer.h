#ifndef __okeya__BGMPlayer__
#define __okeya__BGMPlayer__

#include "audio/include/AudioEngine.h"
#include "cocos2d.h"

using namespace cocos2d;

class BGMPlayer
{
public:
    static void play(const std::string& path);
    static void play2d(const std::string& path);
    static void volume(const int percent);
    static const float getVolume();
    static void stopAll();

private:
    static std::string _currentMusic;
    static int _currentAudioId;
    static float _volume;
};

#endif /* defined(__okeya__BGMPlayer__) */
