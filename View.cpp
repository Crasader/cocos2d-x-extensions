#include "View.h"
#include "cocostudio/CocoStudio.h"
#include "Factory.h"
#include "ViewModel.h"
#include "PluginManager.h"
#include "Env.h"

using namespace cocos2d::plugin;
using namespace cocostudio;

View::View() : _pRootViewModel(nullptr)
{
}

View::~View()
{
    if(_pRootViewModel){
        _pRootViewModel->release();
        _pRootViewModel = nullptr;
    }
}

bool View::initWithFactory(const std::string jsonFname, Factory<ViewModel>& factory)
{
    if ( !Layer::init() ) {
        return false;
    }
    _pRootNode = SceneReader::getInstance()->createNodeWithSceneFile(jsonFname.c_str());
    _pRootViewModel = factory.create("root");
    _pRootViewModel->retain();
    _pRootViewModel->setRoot(_pRootViewModel);
    _pRootViewModel->setNode(this);
    _pRootViewModel->bind(_pRootNode, factory);
    _pRootViewModel->init();
    addChild(_pRootNode);
    setTouchParticle();
//    this->schedule(schedule_selector(View::update));
    return true;
}

void View::setTouchParticle()
{
    if(_pRootNode->getChildrenCount() < 1){
        return;
    }
    const auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* pTouch, Event* pEvent){
        const auto location = pTouch->getLocation();
        auto* pParticle = ParticleSystemQuad::create("particles/Galaxy.plist");
        pParticle->setPosition(location);
        pParticle->setGlobalZOrder(100.0f);
        pParticle->setScale(0.5);
        pParticle->setDuration(0.2f);
        pParticle->setAutoRemoveOnFinish(true);
        addChild(pParticle);
        return true;
    };
    auto pBackground = _pRootNode->getChildren().at(0);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pBackground);
}

void View::runNextAction()
{
    if(_actionQueue.size() > 0){
        auto t = _actionQueue.front();
        Node* pNode = std::get<0>(t);
        auto pAction = std::get<1>(t);
        pNode->runAction(pAction);
        pAction->release();
        _actionQueue.pop();
    }
}

void View::addActionQueue(Node* pNode, FiniteTimeAction* pAction)
{
    pAction->retain();
    _actionQueue.push(std::make_tuple(pNode, pAction));
}

void View::onExit()
{
    Layer::onExit();
    if(_pRootViewModel != nullptr){
        _pRootViewModel->onExit();
    }
}

void View::onEnter()
{
    Layer::onEnter();
    if(_pRootViewModel != nullptr){
        _pRootViewModel->onEnter();
    }
}

void View::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if(_pRootViewModel != nullptr){
        _pRootViewModel->onEnterTransitionDidFinish();
    }
}

void View::showAds()
{
    auto pluginManager = PluginManager::getInstance();
    
    auto plugin = pluginManager->loadPlugin("AdsAdmob");
    _admob = dynamic_cast<ProtocolAds*>(plugin);
    TAdsDeveloperInfo devInfo;

    devInfo["AdmobID"] = ADMOB_ID;
    _admob->configDeveloperInfo(devInfo);
#ifdef DEBUG
    _admob->setDebugMode(true);
#endif
    _adInfo["AdmobType"] = "1";
    _adInfo["AdmobSizeEnum"] = "1";
    
    _admob->showAds(_adInfo, ProtocolAds::kPosBottom);
}

void View::hideAds()
{
    _admob->hideAds(_adInfo);
}

void MyAdsListener::onAdsResult(AdsResultCode code, const char* msg)
{
    log("OnAdsResult, code : %d, msg : %s", code, msg);
}

void MyAdsListener::onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points)
{
    log("Player get points : %d", points);
    
    // @warning should add code to give game-money to player here
    
    // spend the points of player
    if (pAdsPlugin != NULL) {
        pAdsPlugin->spendPoints(points);
    }
}

