#include "BGMPlayer.h"

std::string BGMPlayer::_currentMusic = "";
int BGMPlayer::_currentAudioId = -1;
int BGMPlayer::_volume = 100;

using namespace experimental;

void BGMPlayer::play(const std::string& path)
{
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

void BGMPlayer::play2d(const std::string& path)
{
    float rate = static_cast<float>(BGMPlayer::_volume) / 100;
    AudioEngine::play2d(path, false, rate);
}

void BGMPlayer::setVolume(const int volume)
{
    float rate = static_cast<float>(volume) / 100;
    AudioEngine::setVolume(_currentAudioId, rate);
    BGMPlayer::_volume = volume;
}

void BGMPlayer::stopAll()
{
    AudioEngine::stopAll();
}

const float BGMPlayer::getVolume()
{
    return BGMPlayer::_volume;
}