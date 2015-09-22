/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SGE_TEXLAB_RGB_TO_LUMINANCE_H
#define SGE_TEXLAB_RGB_TO_LUMINANCE_H
#include "sge_ui/wins.h"
#include "sge_ui/image_box.h"
#include "sge_ui/connector.h"
#include "TextureGenerator.h"

class sge_texlab_rgb_to_luminance : public Win
{
public:
    sge_texlab_rgb_to_luminance(WContainer *par);

    void Draw() const;
    void Update(const GameTimer &gt, const MouseState &ms) override;
    ImageBox *ib;
    Connector<std::shared_ptr<Texture>> *in;
    Connector<std::shared_ptr<Texture>> *out;
    TextureGenerator tg;
};

#endif // SGE_TEXLAB_RGB_TO_LUMINANCE_H
