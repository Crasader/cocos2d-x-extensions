//
//  ViewModelCounter.cpp
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2015/08/28.
//
//

#include "ViewModelCounter.h"
#include "SupportFunctions.h"
#include "ui/UIText.h"

using namespace cocos2d::ui;

ViewModelCounter::~ViewModelCounter()
{
}

FiniteTimeAction* ViewModelCounter::countUpOrDown(int count, const int amount)
{
    auto countAction = CallFuncN::create([count, amount](Node* pNode) mutable {
        if(count < 1){
            return;
        }
        auto pCounter = static_cast<Text*>(pNode);
        int counter = std::atoi(pCounter->getString().c_str());
        counter += amount;
        pCounter->setString(supportfunctions::to_string(counter));
        count --;
    });
    float fulltime = 0.4f;
    float time = fulltime / count;
    auto countSeq = Sequence::create(countAction, DelayTime::create(time), nullptr);
    auto countRepeat = Repeat::create(countSeq, count);
    
    return countRepeat;
 }

FiniteTimeAction* ViewModelCounter::countUp(const int count)
{
    return countUpOrDown(count, +1);
}

FiniteTimeAction* ViewModelCounter::countDown(const int count)
{
    return countUpOrDown(count, -1);
}
