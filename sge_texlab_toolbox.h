/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SGE_TEXLAB_TOOLBOX_H
#define SGE_TEXLAB_TOOLBOX_H
#include "sge_ui/wins.h"
#include "sge_ui/list_container.h"
#include "sge_ui/label.h"

#include "sge_renderbox.h"
#include "sge_texlab_rgb_to_luminance.h"
#include "sge_texture_lab.h"
#include "sge_texture_lab_float_selector.h"
#include "sge_texlab_lerp_by_map.h"
#include "sge_ui/treeview.h"

class sge_texlab_toolbox : public Win
{
public:
    sge_texlab_toolbox(WContainer *par);

    void Draw() const;
    void Update(const GameTimer &gt, const MouseState &ms) override;
    ListContainer *lc;
    TreeView *tree;
};

#endif // SGE_TEXLAB_TOOLBOX_H
