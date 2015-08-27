//
//  ViewModelCounter.h
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2015/08/28.
//
//

#ifndef __okeya__ViewModelCounter__
#define __okeya__ViewModelCounter__

#include "cocos2d.h"
#include "SupportFunctions.h"
#include "ui/UIText.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class ViewModelCounter
{
public:
    virtual ~ViewModelCounter();
    virtual FiniteTimeAction* countUp(const int count);
    virtual FiniteTimeAction* countDown(const int count);
    virtual FiniteTimeAction* countUpOrDown(int count, const int amount);
};

#endif /* defined(__okeya__ViewModelCounter__) */
