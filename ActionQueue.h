#ifndef __okeya__ActionQueue__
#define __okeya__ActionQueue__

#include "cocos2d.h"

using namespace cocos2d;

class ActionQueue
{
public:
    typedef Vector<FiniteTimeAction*> Actions;
    static ActionQueue* getInstance();
    void add(Node* pNode, FiniteTimeAction* pAction);
    void add(Node* pNode, Actions& arrayOfActions);
    void run();
private:
    std::queue< std::tuple<Node*, FiniteTimeAction*> > _queue;
};

#endif /* defined(__okeya__ActionQueue__) */
