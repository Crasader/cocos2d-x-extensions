#ifndef __okeya__ViewModel__
#define __okeya__ViewModel__

#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include <spine/SkeletonAnimation.h>
#include "Factory.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;
using namespace cocostudio;
using namespace spine;

class View;

class ViewModel : public Ref
{
public:
    enum Status { POPVIEW = 0 };
    struct ListConfig{
        std::string listTemplate = "PanelSummary";
        Vec2 origin = Vec2(170, 400);
        std::string contentAreaName = "ScrollArea";
        cocos2d::Size contentAreaSize = cocos2d::Size(640, 480);
        int verticalMargin = 130;
    };
    ViewModel();
    virtual ~ViewModel();
    static const int DefaultSize;
    
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
    virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
    virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
    
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    
    virtual Vector<ViewModel*>& getChildren();
    virtual ViewModel* getChildByName(const std::string& name);
    virtual ViewModel* getParent();
    virtual ViewModel* getRoot(const std::string& name, Scene* scene = nullptr);
    virtual View* getView();
    void observeEvent();
    const cocos2d::Rect getBoundingBox() const;
    void appendNode(Node* pNode);
    virtual void bind(Node* children, Factory<ViewModel>& factory);
    virtual void update(const Value& value);
    virtual void refresh(const int status);
    virtual void refresh(const int status, const std::string& callee);
    virtual void set(const std::string& name, const std::string& value);
    virtual void set(const std::string& name, const int value);
    virtual int get(const std::string& name);
    virtual void enable(const std::string& name);
    virtual void disable(const std::string& name);
    virtual void enable();
    virtual void disable();
    virtual void hide();
    virtual void hide(const std::string& name);
    virtual void show();
    virtual void show(const std::string& name);
    virtual void toggle(const std::string& name);
    virtual void toggle(Node* pNode);
    virtual void setList(Factory<ViewModel>& factory, ValueVector* pVec, const ListConfig& config);
    virtual void setList(const std::string& listTemplateName, Factory<ViewModel>&factory, const ValueVector& array, bool hasBlank = true, bool hasScrollBar = false);
    virtual void setList(const std::string& areaName, const std::string& className, const std::string& listTemplateName, Factory<ViewModel>&factory, const ValueVector& array, bool hasBlank = true, bool hasScrollBar = true);
    virtual void setTable(const std::string& listTemplateName, Factory<ViewModel>&factory, const ValueVector& array);
    virtual void setTable(const std::string& areaName, const std::string& className, const std::string& listTemplateName, Factory<ViewModel>&factory, const ValueVector& array, bool hasMargin = true, bool hasScrollBar = false);
    void jumpToTop(cocos2d::ui::ScrollView* pList);
    void disableTouch();
    void disableTouch(const std::string& name);
    void enableTouch();
    void enableTouch(const std::string& name);
    void setSize(const std::string& name, const int value);
    void setNode(Node* pNode);
    void setRoot(ViewModel* pVm);
    int random(const int max);
    Node* getNode(const std::string& name);
    Node* getNode();
    virtual void save(const std::string& name, int value);
    virtual int load(const std::string& name);
    void setName(const std::string name);
    const std::string getName() const;
    spine::SkeletonAnimation* replaceToAnimation(const std::string& nodeName, const std::string& animationName);
    void onTouch(cocos2d::ui::Widget* pWidget, std::function<void (Ref* pRef)> fn);
protected:
    virtual ViewModel* bindInstance(Factory<ViewModel>& factory, Node* pNode, const std::string& name, bool customEventDispatcher = true);
    virtual void removeFromParent(ViewModel*);
    void bindToggle(Factory<ViewModel>& factory, Node* pNode);
    void bindLink(Factory<ViewModel>& factory, Node* pNode);

    static const std::string LabelPrefix;
    static const std::string LinkPhysicsPrefix;
    static const std::string SpritePrefix;
    static const std::string ProgressPrefix;
    
    static const std::vector<std::string> ObserveSuffixes;
    static const std::vector<std::string> ObservePrefixes;
    std::unordered_map<std::string, Node*> _watches;
    std::string _name;
    ViewModel* _pRoot;
    ViewModel* _pParent;
    Node* _pNode;
    cocos2d::Rect _rect;
    Vector<ViewModel*> _children;
    ValueMap _data;
    virtual bool fixName(Node* pNode);
    bool isRectContains(Node* pNode, const Vec2& point);
    View* pushView(const std::string& fname, Factory<ViewModel>& factory, Scene* scene = nullptr, Node* hideNode = nullptr);
    void popView();
    static std::stack<Node*> invisibleNodes;
private:
    void setParent(ViewModel* pParent);
    void operator =(const ViewModel& src);
    ViewModel(const ViewModel& src);
    bool _moved;
};
#endif /* defined(__okeya__ViewModel__) */
