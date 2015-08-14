#ifndef SGE_TEXTURE_LAB_FLOAT_SELECTOR_H
#define SGE_TEXTURE_LAB_FLOAT_SELECTOR_H
#include "sge_ui/wins.h"
#include "sge_ui/dragnumbox.h"
#include "sge_ui/connector.h"

class sge_texlab_float_selector : public Win
{
public:
    sge_texlab_float_selector(WContainer *par);

    void Draw() const;
    void Update(const GameTimer &gt);
    DragNumbox *d;
    Connector<float> *c;

};

#endif // SGE_TEXTURE_LAB_FLOAT_SELECTOR_H
