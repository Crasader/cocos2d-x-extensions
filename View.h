#ifndef __okeya__View__
#define __okeya__View__

#include "cocos2d.h"
#include "ViewModel.h"
#include "Factory.h"
#include "BGMPlayer.h"
#include "ScrollViewModel.h"
#include "ProtocolAds.h"

using namespace cocos2d::plugin;
using namespace cocos2d;

class MyAdsListener : public cocos2d::plugin::AdsListener
{
public:
    virtual void onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg);
    virtual void onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points);
};

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
    std::queue< std::tuple<Node* , FiniteTimeAction*> > _actionQueue;
    virtual void runNextAction();
    virtual void addActionQueue(Node* pNode, FiniteTimeAction * pAction);
    void showAds();
    void hideAds();
private:
    ProtocolAds* _admob;
    TAdsInfo _adInfo;
    void setTouchParticle();
};

#endif /* defined(__okeya__View__) */
