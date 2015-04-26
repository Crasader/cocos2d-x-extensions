//
//  HealByTime.h
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2014/12/12.
//
//

#ifndef __zeron__HealByTime__
#define __zeron__HealByTime__

#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>

class HealByTime
{
public:
    static const std::string MAXLABEL;
    HealByTime(const int maxCount, const int maxSec, const int count);
    void progress(const float dt, const int amount);
    const int getCount() const;
    const int getCurrentSec() const;
    const int getMaxSec() const;
    void setCurrentSec(const int sec);
    const int getMaxCount() const;
    void setCount(const int c);
    const std::string restTime();
private:
    const int _maxCount;
    const int _maxSec;
    int _count;
    int _sec;
    float _dt;
    bool _active;
};

#endif /* defined(__zeron__HealByTime__) */
