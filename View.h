#ifndef __oke-ya__View__
#define __oke-ya__View__

#include "ViewModel.h"

using namespace cocos2d;

class View : public Layer
{
public:
    View();
    virtual ~View();
    virtual bool initWithFactory(std::string jsonFname, Factory<ViewModel>& factory);
    virtual void onExit();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    CREATE_FUNC(View);
protected:
    ViewModel* _pRootViewModel;
    Node* _pRootNode;

private:
    void setTouchParticle();
};

#endif /* defined(__oke-ya__View__) */
