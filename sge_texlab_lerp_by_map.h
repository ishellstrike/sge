#ifndef SGE_TEXLAB_LERP_BY_MAP_H
#define SGE_TEXLAB_LERP_BY_MAP_H
#include "sge_ui/wins.h"
#include "sge_ui/image_box.h"
#include "sge_ui/connector.h"
#include "TextureGenerator.h"

class sge_texlab_lerp_by_map : public Win
{
public:
    sge_texlab_lerp_by_map(WContainer *par);

    void Draw() const;
    void Update(const GameTimer &gt);
    ImageBox *ib;
    Connector<std::shared_ptr<Texture>> *map1, *map2, *lerp;
    Connector<std::shared_ptr<Texture>> *out;

    TextureGenerator tg;
};

#endif // SGE_TEXLAB_LERP_BY_MAP_H
