//
//  SceneManager.cpp
//  oke-ya
//
//  Created by Atsushi Yoshida on 2014/08/05.
//
//

#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager* SceneManager::getInstance()
{
    static SceneManager s_SceneManager;
    return &s_SceneManager;
}

Scene* SceneManager::get(const std::string& name){
    return get(name, 0.5f);
}

Scene* SceneManager::get(const std::string& name, float predicate){
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    cocostudio::ActionManagerEx::getInstance()->destroyInstance();
    auto scene = Scene::create();
    auto layer = _scenes.createWithInit(name);
    scene->addChild(layer);
    predicate = 2.0f;
    return TransitionFade::create(predicate, scene);
}

bool SceneManager::transition(const std::string& name)
{
    Director::getInstance()->replaceScene(get(name));
    return true;
}

void SceneManager::add(std::string type, CreatorBase<View> * pCreator)
{
    _scenes.sign(type, pCreator);
}

