/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "sge_texture_lab_float_selector.h"

sge_texlab_float_selector::sge_texlab_float_selector(WContainer *par) :
    Win(par)
{
    text = "float";

    d = new DragNumbox(this);
    d->max = 2;
    d->min = 0.00001;
    d->growth = 0.0001;
    d->pos = {0,0};
    d->size = {70,30};
    size = {90,50};

    resizable = false;

    c = new Connector<float>(this);
    c->passive = false;

    c->pos = {70, 5};
}

void sge_texlab_float_selector::Draw() const
{
    Win::Draw();
}

void sge_texlab_float_selector::Update(const GameTimer &gt, const MouseState &ms)
{
    c->linked_value = d->value;

    Win::Update(gt, ms);
}


