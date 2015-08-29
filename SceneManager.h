#ifndef __okeya__SceneManager__
#define __okeya__SceneManager__

#include <iostream>
#ifndef __okeya__Factory__
#include "Factory.h"
#endif //__okeya__Factory__
#ifndef __okeya__Factory__
#include "Factory.h"
#endif //__okeya__Factory__

#ifndef __okeya__View__
#include "View.h"
#endif //__okeya__View__
#ifndef __okeya__View__
#include "View.h"
#endif //__okeya__View__


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
