#ifndef __okeya__titlescene_SceneViewModel__
#define __okeya__titlescene_SceneViewModel__

#include "ViewModel.h"

namespace titlescene{
    
class TitleSceneViewModel : public ViewModel
{
public:
    bool init();
    void onEnter();
private:
    bool fixName(Node* pNode);
};
    
}

#endif /* defined(__okeya__TitleSceneViewModel__) */
