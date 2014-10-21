#ifndef __oke-ya__ViewModel__
#define __oke-ya__ViewModel__

#include "cocos2d.h"
#include "Factory.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "editor-support/cocostudio/CCArmature.h"
#include "ui/UIText.h"
#include "extensions/GUI/CCScrollView/CCScrollView.h"
#include "SupportFunctions.h"
#include "BGMPlayer.h"
#include "Naming.h"
#include "LocalCache.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;

class View;

class ViewModel : public Ref
{
public:
    struct ListConfig{
        std::string listTemplate = "ListBox";
        Vec2 origin = Vec2(170, 400);
        std::string contentAreaName = "w_ScrollArea";
        Size contentAreaSize = Size(640, 480);
        int verticalMargin = 130;
    };
    typedef Vector<FiniteTimeAction*> Actions;
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
    virtual View* getView();
    void observeEvent();
    const Rect getBoundingBox() const;
    void appendNode(Node* pNode);
    virtual void bind(Node* children, Factory<ViewModel>& factory);
    virtual void update(Value& value);
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
    virtual void setList(Factory<ViewModel>& factory, ValueVector* pVec, const ListConfig& config);
    virtual void setList(const std::string& areaName, const std::string& listTemplateName, Factory<ViewModel>&factory, ValueVector& array);
    virtual void setDetail(const std::string& templalteName,
                           ValueMap* pVmap, Factory<ViewModel>& factory, std::function<void(ViewModel*, ValueMap*)>);
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

protected:
    virtual ViewModel* bindInstance(Factory<ViewModel>& factory, Node* pNode, const std::string& name);
    virtual void countUp(const std::string& name, const int count);
    void bindToggle(Factory<ViewModel>& factory, Node* pNode);
    void bindLink(Factory<ViewModel>& factory, Node* pNode);

    static const std::string LabelPrefix;
    static const std::string SpritePrefix;
    static const std::string ProgressPrefix;
    
    static const std::vector<std::string> ObserveSuffixes;
    std::map<std::string, Node*> _watches;
    std::string _name;
    ViewModel* _pRoot;
    ViewModel* _pParent;
    Node* _pNode;
    Rect _rect;
    Vector<ViewModel*> _children;
    ValueMap _data;
    virtual bool fixName(Node* pNode);
    
    std::queue< std::tuple<Node*, FiniteTimeAction*> > _actionQueue;
    virtual void runNextAction();
    inline void actionStart(){ runNextAction(); };
    virtual void addActionQueue(Node* pNode, FiniteTimeAction* pAction);
    virtual void addActionQueue(FiniteTimeAction* pAction);
    virtual void addActionQueue(Node* pNode, Actions& arrayOfActions);
    bool isRectContains(Node* pNode, const Vec2& point);

private:
    void setParent(ViewModel* pParent);
    void operator =(const ViewModel& src);
    ViewModel(const ViewModel& src);
};
#endif /* defined(__oke-ya__ViewModel__) */
