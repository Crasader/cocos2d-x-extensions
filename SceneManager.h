#ifndef __okeya__SceneManager__
#define __okeya__SceneManager__

#include <iostream>
#include "Factory.h"
#include "View.h"
#include "cocos2d.h"

using namespace cocos2d;

class SceneManager
{
public:
    static SceneManager* getInstance();
    Scene* get(const std::string& name, float predicate);
    Scene* get(const std::string& name);
    bool transition(const std::string& name);
    void add(std::string type, CreatorBase<View> * pView);
private:
    SceneManager();
    SceneManager(const SceneManager &sceneManager);
    static SceneManager* _singleton;
    Factory<View> _scenes;
};

#endif /* defined(__okeya__SceneManager__) */
