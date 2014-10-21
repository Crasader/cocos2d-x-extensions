#include "View.h"
#include "cocostudio/CocoStudio.h"
#include "Factory.h"
#include "ViewModel.h"

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
