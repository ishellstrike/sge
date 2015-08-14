#ifndef SGE_RENDERBOX_H
#define SGE_RENDERBOX_H
#include "sge_ui/wins.h"
#include "sge_ui/graph.h"
#include "sge_ui/label.h"
#include "sge_ui/image_box.h"
#include "sge_ui/dragnumbox.h"
#include "sge_ui/connector.h"
#include "sge_ui/label.h"

class sge_texlab_renderbox : public Win
{
public:
    sge_texlab_renderbox(WContainer *par);

    void Draw() const;
    void Update(const GameTimer &gt);
    ImageBox *ib;
    Connector<std::shared_ptr<Texture>> *in;
    Label *label;
};

#endif // SGE_RENDERBOX_H

