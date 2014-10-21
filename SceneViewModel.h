#ifndef __oke-ya__titlescene_SceneViewModel__
#define __oke-ya__titlescene_SceneViewModel__

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

#endif /* defined(__oke-ya__TitleSceneViewModel__) */
