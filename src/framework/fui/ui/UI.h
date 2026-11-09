/** 
 * UI.h 
 * created 11/08/26 by frank collebrusco
 */
#ifndef UI_H
#define UI_H
#include <flgl/glm.h>
#include "../data/UIData.h"
#include "../render/UIRenderer.h"
#include "UICore.h"


class RootUIelement : public UIelement {
    void center();
    virtual void on_layout() override;
public:
    RootUIelement();
};



#endif /* UI_H */
