#include "BGMPlayer.h"

std::string BGMPlayer::_currentMusic = "";
int BGMPlayer::_currentAudioId = -1;
float BGMPlayer::_volume = 1.0f;

using namespace experimental;

void BGMPlayer::play(const std::string& path)
{
    if(_currentMusic == path){
        return;
    }
    if(_currentMusic.size() > 0 && _currentMusic != path){
        AudioEngine::stop(_currentAudioId);
    }
    _currentAudioId = AudioEngine::play2d(path, true, BGMPlayer::_volume);
    _currentMusic = path;
}

void BGMPlayer::play2d(const std::string& path)
{
    AudioEngine::play2d(path, false, BGMPlayer::_volume);
}

void BGMPlayer::volume(const int percent)
{
    float volume = static_cast<float>(percent) / 100;
    AudioEngine::setVolume(_currentAudioId, volume);
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