#include "BGMPlayer.h"

std::string BGMPlayer::_currentMusic = "";
int BGMPlayer::_currentAudioId = -1;

using namespace experimental;

void BGMPlayer::play(const std::string& path)
{
    if(_currentMusic == path){
        return;
    }
    if(_currentMusic.size() > 0 && _currentMusic != path){
        AudioEngine::stop(_currentAudioId);
    }
    _currentAudioId = AudioEngine::play2d(path, true, 1.0f);
    _currentMusic = path;
}
