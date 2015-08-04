#include "BGMPlayer.h"

std::string BGMPlayer::_currentMusic = "";
std::string BGMPlayer::_prevSE = "";
int BGMPlayer::_currentAudioId = -1;
int BGMPlayer::_volume = 100;
int BGMPlayer::_volumeSE = 100;
bool BGMPlayer::_mute = false;

using namespace experimental;

void BGMPlayer::play(const std::string& path)
{
    if(_mute){
        return;
    }
    if(_currentMusic == path){
        return;
    }
    if(_currentMusic.size() > 0 && _currentMusic != path){
        AudioEngine::stop(_currentAudioId);
    }
    float rate = static_cast<float>(BGMPlayer::_volume) / 100;
    _currentAudioId = AudioEngine::play2d(path, true, rate);
    _currentMusic = path;
}

int BGMPlayer::play2d(const std::string& path)
{
    if(_mute){
        return 0;
    }
    float rate = static_cast<float>(BGMPlayer::_volumeSE) / 100;
    return AudioEngine::play2d(path, false, rate);
}

void BGMPlayer::play2du(const std::string& path)
{
    if(_prevSE == path){
        return;
    }
    _prevSE = path;
    auto audioId = play2d(path);
    AudioEngine::setFinishCallback(audioId, [](int, const std::string &){
        BGMPlayer::_prevSE = "";
    });
}

void BGMPlayer::setVolume(const int volume)
{
    float rate = static_cast<float>(volume) / 100;
    AudioEngine::setVolume(_currentAudioId, rate);
    BGMPlayer::_volume = volume;
}

void BGMPlayer::setVolumeSE(const int volume)
{
    BGMPlayer::_volumeSE = volume;
}

void BGMPlayer::stopAll()
{
    _currentAudioId = -1;
    _currentMusic = "";
    AudioEngine::stopAll();
}
