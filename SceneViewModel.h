#ifndef __okeya__titlescene_SceneViewModel__
#define __okeya__titlescene_SceneViewModel__

#ifndef __okeya__ViewModel__
#include "ViewModel.h"
#endif //__okeya__ViewModel__
#ifndef __okeya__ViewModel__
#include "ViewModel.h"
#endif //__okeya__ViewModel__


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
