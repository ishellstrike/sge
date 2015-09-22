/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SGE_TEXTURE_LAB_H
#define SGE_TEXTURE_LAB_H
#include "sge_ui/wins.h"
#include "sge_ui/image_box.h"
#include "sge_ui/connector.h"
#include "TextureGenerator.h"

class sge_texlab_heightmap : public Win
{
public:
    sge_texlab_heightmap(WContainer *par);

    void Draw() const;
    void Update(const GameTimer &gt, const MouseState &ms) override;
    ImageBox *ib;
    Connector<float> *scale, *offsetx, *offsety;
    Connector<std::shared_ptr<Texture>> *out;
    TextureGenerator tg;
};

#endif // SGE_TEXTURE_LAB_H
