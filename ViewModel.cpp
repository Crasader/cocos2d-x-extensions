#include "ViewModel.h"
#include "ui/CocosGUI.h"
#include "SceneManager.h"
#include "SupportFunctions.h"
#include "editor-support/cocostudio/CocoStudio.h"

using namespace experimental;
using namespace extension;
using namespace cocos2d::ui;

const std::string ViewModel::LabelPrefix  = "s_";
const std::string ViewModel::SpritePrefix = "i_";
const std::string ViewModel::ProgressPrefix = "p_";
const std::string ViewModel::LinkPhysicsPrefix = "g_";
const std::vector<std::string> ViewModel::ObserveSuffixes = {"Layer", "Area", "Animation"};
const std::vector<std::string> ViewModel::ObservePrefixes = {"Character"};

const int ViewModel::DefaultSize = 1000;

ViewModel::ViewModel() : _pParent(nullptr)
{
}

ViewModel::~ViewModel()
{
    _children.clear();
    _watches.clear();
    _pParent = nullptr;
    _pNode = nullptr;
}

bool ViewModel::init()
{
    return true;
}

void ViewModel::setRoot(ViewModel* pVm)
{
    _pRoot = pVm;
}

void ViewModel::setParent(ViewModel* pParent)
{
    _pParent = pParent;
}

ViewModel* ViewModel::getParent()
{
    return _pParent;
}

void ViewModel::setNode(Node* pNode)
{
    _pNode = pNode;
}

Vector<ViewModel*>& ViewModel::getChildren()
{
    return _children;
}

void ViewModel::observeEvent()
{
    auto* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* pTouch, Event* pEvent){
        auto pTarget = pEvent->getCurrentTarget();
        auto pWidget = static_cast<Widget*>(pTarget);
        if(!pWidget->isEnabled()){
            log("%s is disabled.", pTarget->getName().c_str());
            return false;
        }
        if(isRectContains(pTarget, pTouch->getLocation())){
            return onTouchBegan(pTouch, pEvent);
        }
        return false;
    };
    listener->onTouchMoved = [&](Touch* pTouch, Event* pEvent){
        onTouchMoved(pTouch, pEvent);
    };
    listener->onTouchEnded = [&](Touch* pTouch, Event* pEvent){
        onTouchEnded(pTouch, pEvent);
    };
    auto pWidget = static_cast<Widget*>(_pNode);
    pWidget->setTouchEnabled(false);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _pNode);
}

bool ViewModel::isRectContains(Node* pNode, const Vec2& point)
{
    auto touchLocation = pNode->convertToNodeSpace(point);
    auto s = pNode->getContentSize();
    auto rect = Rect(0, 0, s.width, s.height);
    return rect.containsPoint(touchLocation);
    if(static_cast<Widget*>(pNode)->getRotation() == 0){
        if(rect.containsPoint(touchLocation)){
            return true;
        }else{
            return false;
        }
    }
//
//    auto size = pNode->getContentSize();
//    Point v[4] = {
//        Point(0,0),
//        Point(0, size.height),
//        Point(size.width, size.height),
//        Point(size.width, 0),
//    };
//    
//    auto affine = pNode->getNodeToParentAffineTransform();
//    auto name = pNode->getName();
//    for(int i = 0; i < 4; i++) {
//        v[i] = PointApplyAffineTransform(v[i], affine);
//    }
//    Point vec[4] = {
//        v[1] - v[0],
//        v[2] - v[1],
//        v[3] - v[2],
//        v[0] - v[3],
//    };
//    
//    Point vecP[4] = {
//        v[0] - touchLocation,
//        v[1] - touchLocation,
//        v[2] - touchLocation,
//        v[3] - touchLocation,
//    };
//    
//    for(int i = 0; i < 4; i++){
//        if((vec[i].x * vecP[i].y - vec[i].y * vecP[i].x) < 0){
//            return false;
//        }
//    }
    return true;
}

bool ViewModel::onTouchBegan(Touch* pTouch, Event* pEvent){
    log("ViewModel::onTouchBegan, %s", pEvent->getCurrentTarget()->getName().c_str());
    return false;
}

void ViewModel::onTouchMoved(Touch* pTouch, Event* pEvent){
    log("ViewModel::onTouchMoved, %s", pEvent->getCurrentTarget()->getName().c_str());
}

void ViewModel::onTouchEnded(Touch* pTouch, Event* pEvent){
    log("ViewModel::onTouchEnded, %s", pEvent->getCurrentTarget()->getName().c_str());
}

const Rect ViewModel::getBoundingBox() const
{
    return _pNode->getBoundingBox();
}

void ViewModel::appendNode(Node* pNode)
{
    _pNode->getParent()->addChild(pNode);
}

void ViewModel::bind(Node* pNode, Factory<ViewModel>& factory)
{
    if(pNode == nullptr){
        return;
    }
        
    _pRoot->fixName(pNode);
    static const std::string instancePrefix = "c_";
    static const std::string togglePrefix = "t_";
    static const std::string linkPrefix = "l_";

    static const std::string animationPrefix = "a_";
    static const std::string watchPrefix = "w_";
    static const std::string sep = "_";
    static const std::string parentSep = ".";
    const std::string parentName = pNode->getName();

    auto const name = pNode->getName();
//    log("%s, %x", name.c_str(), pNode->isVisible());
    auto prefix = name.substr(0, 2);
    if(prefix == instancePrefix){
        auto i = name.find_last_of(sep);
        if(i < 2){
            i = -1;
        }
        auto const signName = name.substr(0, i);
        bindInstance(factory, pNode, signName);
        return;
    }else if(prefix == togglePrefix){
        bindToggle(factory, pNode);
    }else if(prefix == linkPrefix){
        bindLink(factory, pNode);
    }else if(prefix == ViewModel::LinkPhysicsPrefix){
        bindLink(factory, pNode);
    }else if(prefix == watchPrefix || prefix == ViewModel::ProgressPrefix){
        _watches.insert({name, pNode});
    }else if(prefix == ViewModel::LabelPrefix  ||
             prefix == ViewModel::SpritePrefix ||
             prefix == animationPrefix){
        
        if(prefix == ViewModel::LabelPrefix){
            auto pText = static_cast<Text*>(pNode);
            if(pText->getType() == Text::Type::TTF){
                auto color = pText->getColor();
                pText->enableShadow();
                pText->setTextColor(Color4B(color.r, color.g, color.b, 255));
            }
        }
        
        auto posItr = _watches.find(name);
        if(posItr == _watches.end()){
            _watches.insert({name, pNode});
        }else{
            _watches.insert({pNode->getParent()->getName() + "." + name, pNode});
            auto pExisted = std::get<1>(*posItr);
            auto k = pExisted->getParent()->getName() + "." + pExisted->getName();
            auto posItr2 = _watches.find(k);
            if(posItr2 == _watches.end()){
                _watches.insert({k, pExisted});
            }
        }
    }else{
        for(auto& observeSuffix: ViewModel::ObserveSuffixes){
            auto i = name.find_last_of(observeSuffix.at(0));
            if(i == std::string::npos){
                continue;
            }
            std::string suffix = name.substr(i, -1);
            if(suffix == observeSuffix){
                _watches.insert({name, pNode});
                break;
            }
        }
        for(auto& observePrefix: ViewModel::ObservePrefixes){
            std::string prefix = name.substr(0, observePrefix.size());
            if(prefix == observePrefix){
                _watches.insert({name, pNode});
                break;
            }
        }
    }
    auto children = pNode->getChildren();
    for(auto& pChild: children){
        bind(pChild, factory);
    }
}

ViewModel* ViewModel::bindInstance(Factory<ViewModel>& factory, Node* pNode, const std::string& name)
{
    auto* pViewModel = factory.create(name);
    CCASSERT(pViewModel, ("factory " + name + " is not defined").c_str());
    pViewModel->setName(pNode->getName());
    pViewModel->setParent(this);
    pViewModel->setRoot(_pRoot);
    pViewModel->setNode(pNode);
    auto children = pNode->getChildren();
    for(auto& pChild: children){
        pViewModel->bind(pChild, factory);
    }
    pViewModel->observeEvent();
    pViewModel->init();
    this->getChildren().pushBack(pViewModel);
    return pViewModel;
}

void ViewModel::removeFromParent(ViewModel* pViewModel)
{
    auto& children = getChildren();
    ssize_t index = children.getIndex(pViewModel);
    if( index != CC_INVALID_INDEX ){
        pViewModel->getNode()->removeFromParent();
        pViewModel->setParent(nullptr);
        pViewModel->release();
        children.erase(index);
    }

}

void ViewModel::bindToggle(Factory<ViewModel>& factory, Node* pNode)
{
    auto pWidget = static_cast<Widget*>(pNode);
    pWidget->addTouchEventListener([&](Ref* pRef, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::BEGAN){
            AudioEngine::play2d("sound/se_button_push_01.mp3");
            auto pNode = static_cast<Node*>(pRef);
            auto name = pNode->getName().substr(2, -1);
            auto pTarget = getNode(name);
            toggle(pTarget);
        }
    });
    pWidget->setSwallowTouches(true);
}

void ViewModel::bindLink(Factory<ViewModel>& factory, Node* pNode)
{
    auto pWidget = static_cast<Widget*>(pNode);
    auto scale = ScaleBy::create(0.8f, 1.15f);
    auto action = Repeat::create(Sequence::create(scale, scale->reverse(), nullptr), -1);
    auto name = pNode->getName();
    if (name == "l_CostumeRoomScene" || name == "g_SummonRoomScene" ||
        name == "l_MyRoomScene" || name == "l_WorkScene") {
        auto scale = ScaleBy::create(0.8f, 1.15f);
        auto wait  = DelayTime::create(0.8f);
        auto brightOn  = CallFuncN::create([](Node* pNode){
            static_cast<Button*>(pNode)->setBrightStyle(BrightStyle::HIGHLIGHT);
        });
        auto brightOff = CallFuncN::create([](Node* pNode){
            static_cast<Button*>(pNode)->setBrightStyle(BrightStyle::NORMAL);
        });
        action = Repeat::create(
             Sequence::create(wait, wait, brightOn, wait, wait,
                              brightOff, wait, scale, scale->reverse(),
                              scale, scale->reverse(), nullptr), -1);
    }
    pNode->stopAllActions();
    pNode->setScale(1.0f);
    pNode->runAction(action);
    pWidget->addTouchEventListener([&](Ref* pRef, Widget::TouchEventType type){
        if(type == Widget::TouchEventType::BEGAN){
            disableTouch();
            AudioEngine::play2d("sound/se_button_push_01.mp3");
            auto pNode = static_cast<Node*>(pRef);
            auto prefix = pNode->getName().substr(0, 2);
            auto name = pNode->getName().substr(2, -1);
            Scene* scene;
            if(prefix == ViewModel::LinkPhysicsPrefix){
                scene = SceneManager::getInstance()->get(name, -1.0f);
            }else{
                scene = SceneManager::getInstance()->get(name);
            }

            Director::getInstance()->replaceScene(scene);
        }
    });
}

Node* ViewModel::getNode(const std::string& name)
{
    auto pNode = _watches[name];
    if(pNode == nullptr){
        std::queue<Node*> targets;
        Node* pNode = _pRoot->getNode();
        targets.push(pNode);
        while(targets.size() > 0){
            auto children = pNode->getChildren();
            for(auto pChild: children){
                if(pChild->getName() == name){
                    _watches[name] = pChild;
                    return pChild;
                }else{
                    targets.push(pChild);
                }
            }
            targets.pop();
            pNode = targets.front();
        }
    }
    return pNode;
}

Node* ViewModel::getNode()
{
    return _pNode;
}

void ViewModel::set(const std::string& name, const std::string& value){
    auto pos = name.find_first_of(".");
    std::string prefix;
    if(pos != std::string::npos){
        prefix = name.substr(pos + 1, 2);
    }else{
        prefix = name.substr(0, 2);
    }
    auto pNode = getNode(name);
    if(pNode == nullptr){
        auto pParent = getParent();
        if(pParent == nullptr){
            auto children = getChildren();
            for(auto child: children){
                pNode = child->getNode(name);
                if(pNode){ break; }
            }
        }else{
            pNode = getParent()->getNode(name);
        }
        
    }
    CCASSERT(pNode, ("Invalid node name " + name).c_str());
    if(prefix == ViewModel::SpritePrefix){
        auto pSprite = static_cast<ImageView*>(pNode);

        if(value == ""){
            pSprite->setVisible(false);
        }else{
            pSprite->loadTexture(value);
            pSprite->setVisible(true);
        }
    }else if(prefix == ViewModel::LabelPrefix){
        auto pText = static_cast<Text*>(pNode);
        pText->setString(value);
    }else if(prefix == ViewModel::ProgressPrefix){
        static_cast<LoadingBar*>(pNode)->setPercent(std::stoi(value));
    }else{
        auto visible = value == "0" ? false : true;
        pNode->setVisible(visible);
    }
}

void ViewModel::set(const std::string& name, int value)
{
    set(name, supportfunctions::to_string(value));
}

int ViewModel::get(const std::string& name)
{
    auto str = static_cast<Text*>(getNode(name))->getString();
    return std::stoi(str);
}

void ViewModel::setSize(const std::string& name, const int value)
{
    if(value > 0){
        auto pSprite = static_cast<Sprite*>(_watches[name]);
        auto f = float(value) / ViewModel::DefaultSize;
        pSprite->setScale(f);
    }
}

void ViewModel::setList(Factory<ViewModel>& factory, ValueVector* pVec, const ListConfig& config)
{
    auto pLayer = static_cast<cocos2d::ui::ScrollView*>(getNode(config.contentAreaName));
    auto children = pLayer->getChildren();
    for(auto& li : children){
        li->removeFromParent();
    }
    CCASSERT(pLayer, "Missing ScrollArea");
    auto fname = "ui/" + config.listTemplate  + ".ExportJson";
    auto pTmpl = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(fname.c_str()));
    pTmpl->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    int i = 0;
    int y = config.origin.y;
    for(auto& values: (*pVec)){
        auto pClone = pTmpl->clone();
        auto name = "c_li_" + supportfunctions::to_string(i);
        pClone->setName(name);
        pClone->setPosition(Vec2(config.origin.x, y));
        auto box = pClone->getBoundingBox();
        log("%p", &box);
        pLayer->addChild(pClone);
        auto vm = _pRoot->bindInstance(factory, pClone, "li");
        vm->update(values);
        if(pClone->getParent() != nullptr){
            i++;
            y += config.verticalMargin;
        }
    }
    if (y > config.contentAreaSize.height) {
        pLayer->setInnerContainerSize(Size(config.contentAreaSize.width, y - 40));
    } else {
        pLayer->setInnerContainerSize(config.contentAreaSize);
    }
}

void ViewModel::setList(const std::string& areaName, const std::string& listTemplateName, Factory<ViewModel>&factory, ValueVector& array)
{
    auto pLayer = static_cast<cocos2d::ui::ListView*>(getNode(areaName));
    pLayer->removeAllChildren();
    auto pTmpl = static_cast<Widget*>(_pRoot->getNode(listTemplateName));

    int i = 0;
    for(auto& values: array){
        auto pClone = pTmpl->clone();
        auto pNode = pClone->getChildByName("c_SummaryViewModel");
        pLayer->addChild(pClone);
        auto vm = _pRoot->bindInstance(factory, pNode, "c_SummaryViewModel");
        vm->update(values);
        i++;
    }
    auto pPanel = _pRoot->getNode("w_PanelTemplete");
    if (pPanel != nullptr) {
        pPanel->removeFromParent();
        pLayer->refreshView();
    }
}


void ViewModel::setDetail(const std::string& templateName,
                          ValueMap* pVmap, Factory<ViewModel>& factory, std::function<void(ViewModel*, ValueMap*)>callback)
{
    const std::string fname = "ui/" + templateName + "/" + templateName + ".ExportJson";
    const std::string name = "w_Status";
    auto pVm = _pRoot->getChildByName(name);
    if(pVm == nullptr){
        auto pNode = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(fname.c_str());
        pNode->setName(name);
        pNode->setPosition(Vec2(400, 0));
        pNode->getChildByName("TargetStatus")->setName("w_TargetStatus");
        pVm = _pRoot->bindInstance(factory, pNode, "detail");
        _pNode->getParent()->addChild(pNode);
    }
    callback(pVm, pVmap);
}

void ViewModel::countUp(const std::string& iconName, const std::string& countName, const int count)
{
    auto pIcon    = getNode(iconName);
    auto pCounter = getNode(countName);
    auto countAction = CallFuncN::create([&, pIcon](Node* pNode){
        auto pCounter = static_cast<Text*>(pNode);
        int count = std::atoi(pCounter->getString().c_str());
        count ++;
        pCounter->setString(supportfunctions::to_string(count));
        auto scale = ScaleBy::create(0.4f, 1.4f);
        auto action = Sequence::create(scale, scale->reverse(), scale, scale->reverse(), nullptr);
        pIcon->runAction(action);
    });
    auto pRepeat = Repeat::create(Sequence::create(countAction, DelayTime::create(0.02f), nullptr), count);
    getRoot("BattleScene")->addActionQueue(pCounter, pRepeat);
}

void ViewModel::onExit()
{
    log("ViewModel::onExit");
}

void ViewModel::onEnter()
{
    log("ViewModel::onEnter");
}

void ViewModel::onEnterTransitionDidFinish()
{
    log("ViewModel::onEnterTransitionDidFinish");
}

bool ViewModel::fixName(Node* pNode)
{
    return true;
}

void ViewModel::save(const std::string& name, int value)
{
    UserDefault::getInstance()->setIntegerForKey(name.c_str(), value);
}

int ViewModel::load(const std::string& name){
    return UserDefault::getInstance()->getIntegerForKey(name.c_str());
}

View* ViewModel::getView()
{
    if(getParent() == nullptr){
        return static_cast<View*>(_pNode);
    }else{
        return getParent()->getView();
    }
}

void ViewModel::setName(const std::string name){
    _name = name;
}

const std::string ViewModel::getName() const
{
    return _name;
}

ViewModel* ViewModel::getChildByName(const std::string& name)
{
    auto children = getChildren();
    for(auto pChild: children){
        if(pChild->getName() == name){
            return pChild;
        }
    }
    return nullptr;
}

void ViewModel::update(Value& value)
{
}

void ViewModel::runNextAction()
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

void ViewModel::addActionQueue(Node* pNode, FiniteTimeAction* pAction)
{
    auto pSeq = Sequence::createWithTwoActions(pAction, CallFunc::create([&](){ runNextAction(); }));
    pSeq->retain();
    _actionQueue.push(std::make_tuple(pNode, pSeq));
}

void ViewModel::addActionQueue(FiniteTimeAction* pAction)
{
    addActionQueue(getNode(), pAction);
}

void ViewModel::addActionQueue(Node* pNode, Vector<FiniteTimeAction*>& arrayOfActions)
{
    arrayOfActions.pushBack(CallFunc::create([&](){ runNextAction(); }));
    auto pSeq = Sequence::create(arrayOfActions);
    if(pNode){
        pSeq->retain();
        _actionQueue.push(std::make_tuple(pNode, pSeq));
    }
}

void ViewModel::enable(const std::string& name)
{
    static_cast<Widget*>(getNode(name))->setEnabled(true);
}

void ViewModel::disable(const std::string& name)
{
    static_cast<Widget*>(getNode(name))->setEnabled(false);
}

void ViewModel::enable()
{
    static_cast<Widget*>(getNode())->setEnabled(true);
}

void ViewModel::disable()
{
    static_cast<Widget*>(getNode())->setEnabled(false);
}

void ViewModel::hide()
{
    _pNode->setVisible(false);
}

void ViewModel::hide(const std::string& name)
{
    getNode(name)->setVisible(false);
}

void ViewModel::show()
{
    _pNode->setVisible(true);
}

void ViewModel::show(const std::string& name)
{
    getNode(name)->setVisible(true);
}

void ViewModel::disableTouch()
{
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
}

void ViewModel::enableTouch()
{
    Director::getInstance()->getEventDispatcher()->setEnabled(true);
}

void ViewModel::disableTouch(const std::string& name)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(getNode(name), true);
}

void ViewModel::enableTouch(const std::string& name)
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(getNode(name), true);
}

int ViewModel::random(const int max)
{
    std::random_device randomDevice;
    std::vector<uint32_t> randomSeedVector(10);
    std::generate(randomSeedVector.begin(), randomSeedVector.end(), std::ref(randomDevice));
    std::seed_seq randomSeed(randomSeedVector.begin(), randomSeedVector.end());
    std::mt19937 randomEngine(randomSeed);
    std::uniform_int_distribution<int> dist(0, max);
    return dist(randomEngine);
}

void ViewModel::toggle(const std::string& name)
{
    toggle(getNode(name));
}

void ViewModel::toggle(Node* pNode)
{
    if(pNode->isVisible()){
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(pNode, true);
        auto effect = Sequence::create(FadeOut::create(0.2f),
                                       CallFuncN::create([&](Node* pNode){
            auto name = pNode->getName();
            pNode->getParent()->reorderChild(pNode, 0);
            disableTouch(name);
            hide(name);
        }), nullptr);
        pNode->runAction(effect);
    }else{
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(pNode, true);
        auto effect = Sequence::create(FadeIn::create(0.2f),
                                       CallFuncN::create([&](Node* pNode){
            pNode->getParent()->reorderChild(pNode, 100);
            auto name = pNode->getName();
            enableTouch(name);
            show(name);
        }), nullptr);
        pNode->runAction(effect);
    }

}

View* ViewModel::pushView(const std::string& name, Factory<ViewModel>& factory)
{
    auto view = View::create();
    view->initWithFactory(name, factory);
    view->setName(name);
    Director::getInstance()->getRunningScene()->addChild(view);
    return view;
}

void ViewModel::popView()
{
    auto& children = Director::getInstance()->getRunningScene()->getChildren();
    auto* view = static_cast<View*>(children.back());
    view->removeFromParent();
}

ViewModel* ViewModel::getRoot(const std::string& name)
{
    auto& children = Director::getInstance()->getRunningScene()->getChildren();
    for(auto& child: children){
        if(child->getName() == name){
            return static_cast<View*>(child)->getRootViewModel();
        }
    }
    return nullptr;
}

spine::SkeletonAnimation* ViewModel::replaceToAnimation(const std::string& nodeName, const std::string& animationName)
{
    auto skeletonNode = spine::SkeletonAnimation::createWithFile(animationName + ".json",
                                                                 animationName + ".atlas", 1);
    auto base = getNode(nodeName);
    auto pos = base->getPosition();
    skeletonNode->setPosition(base->getPosition());
    skeletonNode->setScale(base->getScale());
    base->getParent()->addChild(skeletonNode);
    base->removeFromParent();
    _watches[nodeName] = skeletonNode;
    return skeletonNode;
}