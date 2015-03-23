//
//  ActionQueue.cpp
//  まじょのおしごと
//
//  Created by Atsushi Yoshida on 2015/03/06.
//
//

#include "ActionQueue.h"

ActionQueue* ActionQueue::getInstance()
{
    static ActionQueue s_instance;
    return &s_instance;
}

void ActionQueue::add(Node* pNode, FiniteTimeAction* pAction)
{
    auto pSeq = Sequence::createWithTwoActions(pAction, CallFunc::create([&](){ run(); }));
    pSeq->retain();
    _queue.push(std::make_tuple(pNode, pSeq));
}

void ActionQueue::add(Node* pNode, Actions& arrayOfActions)
{
    arrayOfActions.pushBack(CallFunc::create([&](){ run(); }));
    auto pSeq = Sequence::create(arrayOfActions);
    if(pNode){
        pSeq->retain();
        _queue.push(std::make_tuple(pNode, pSeq));
    }
}

void ActionQueue::run()
{
    if(_queue.size() > 0){
        auto t = _queue.front();
        Node* pNode = std::get<0>(t);
        pNode->resume();
        auto pAction = std::get<1>(t);
        pNode->runAction(pAction);
        pAction->release();
        _queue.pop();
    }
}
