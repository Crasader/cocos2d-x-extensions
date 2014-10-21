#include "TitleScene.h"
#include "TitleSceneViewModel.h"

using namespace titlescene;

bool TitleScene::init()
{
Factory<ViewModel> factory;

factory.sign("root", new Creator<titlescene::TitleSceneViewModel,  ViewModel>);

if ( !View::initWithFactory("publish/TitleScene.json", factory) )
{
    return false;
}

return true;
}
