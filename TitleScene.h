#ifndef __okeya__TitleScene__
#define __okeya__TitleScene__

#include "cocos2d.h"
#include "View.h"

using namespace cocos2d;

class TitleScene : public View
{
public:
    virtual bool init();
    CREATE_FUNC(TitleScene);
};

#endif /* defined(__okeya__TitleScene__) */
