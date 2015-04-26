//
//  HealByTime.cpp
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2014/12/12.
//
//

#include "HealByTime.h"

const std::string HealByTime::MAXLABEL = "(MAX)";

HealByTime::HealByTime(const int maxCount, const int maxSec, const int count)
: _maxCount(maxCount)
, _maxSec(maxSec)
, _sec(maxSec)
, _count(count)
, _dt(0.0f)
, _active(count <= maxCount)
{
}

void HealByTime::progress(const float dt, const int amount)
{
    if(!_active){
        return;
    }
    _dt += dt;
    if(_dt < (1.0f / amount)){
        return;
    }
    _dt = 0.0f;
    _sec -= 1;
    if(_sec < 1){
        _count ++;
        _sec = _maxSec;
    }

    if(_count > _maxCount){
        _count = _maxCount;
    }
}

const int HealByTime::getCount() const
{
    return _count;
}

void HealByTime::setCurrentSec(const int sec)
{
    _sec = sec;
}

const int HealByTime::getCurrentSec() const
{
    return _sec;
}

const int HealByTime::getMaxSec() const
{
    return _maxSec;
}

const int HealByTime::getMaxCount() const
{
    return _maxCount;
}

void HealByTime::setCount(const int c)
{
    if(c > _maxCount){
        _active = false;
    }else{
        _active = true;
    }
    
    _count = c;
}

const std::string HealByTime::restTime()
{
    if(_count >= _maxCount){
        return HealByTime::MAXLABEL;
    }
    std::stringstream s;
    s << "(";
    s << std::setfill('0') << std::setw(2) << (_sec / 60);
    s << ":";
    s << std::setfill('0') << std::setw(2) << (_sec % 60);
    s << ")";
    return s.str();
}
