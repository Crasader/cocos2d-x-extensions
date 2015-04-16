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
    static void setVolume(const int volume);
    inline static void setMute(bool b){ BGMPlayer::_mute = b; }
    inline static const float getVolume(){ return BGMPlayer::_volume; }
    static void stopAll();

private:
    static std::string _currentMusic;
    static int _currentAudioId;
    static int _volume;
    static bool _mute;
};

#endif /* defined(__okeya__BGMPlayer__) */
