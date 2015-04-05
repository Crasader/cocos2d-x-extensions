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


void ActionQueue::add(std::tuple<Node*, FiniteTimeAction*> tuple)
{
    _queue.push(tuple);
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

void ActionQueue::add(Node* pNode, ActionThread& thread)
{
    auto& last = thread.back();
    thread.pop_back();
    auto n = std::get<0>(last);
    auto a = std::get<1>(last);
    n->release();
    a->release();
    thread.add(n, Sequence::create(a, CallFunc::create([&](){ run(); }), nullptr));

    add(pNode, CallFunc::create([&, thread](){
        for(auto& exp: thread){
            auto& node = std::get<0>(exp);
            auto& action = std::get<1>(exp);
            node->runAction(action);
            action->release();
            node->release();
        }
    }));
}

void ActionThread::add(Node* pNode, FiniteTimeAction* pAction)
{
    pNode->retain();
    pAction->retain();
    push_back(std::make_tuple(pNode, pAction));
}
