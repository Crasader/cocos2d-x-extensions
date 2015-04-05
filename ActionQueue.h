#ifndef __okeya__ActionQueue__
#define __okeya__ActionQueue__

#include "cocos2d.h"

using namespace cocos2d;

class ActionThread : private std::vector< std::tuple<Node*, FiniteTimeAction*> >
{
public:
    void add(Node* pNode, FiniteTimeAction* pAction);
    using std::vector< std::tuple<Node*, FiniteTimeAction*> >::begin;
    using std::vector< std::tuple<Node*, FiniteTimeAction*> >::end;
    using std::vector< std::tuple<Node*, FiniteTimeAction*> >::back;
    using std::vector< std::tuple<Node*, FiniteTimeAction*> >::pop_back;
};

class ActionQueue
{
public:
    typedef Vector<FiniteTimeAction*> Actions;
    static ActionQueue* getInstance();
    void add(Node* pNode, FiniteTimeAction* pAction);
    void add(Node* pNode, Actions& arrayOfActions);
    void add(Node* pNode, ActionThread& thread);
    void add(std::tuple<Node*, FiniteTimeAction*>);
    void run();
private:
    std::queue< std::tuple<Node*, FiniteTimeAction*> > _queue;
};


#endif /* defined(__okeya__ActionQueue__) */
