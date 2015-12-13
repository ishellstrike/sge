/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "textureatlas.h"
#include "sge_fielsystem.h"
#include "resources/pixmap.h"
#include "prefecences.h"
#include "logger.h"
#include "resources/error_image.h"

namespace {
    const int atlas_dim = 2048;
    const int sprite_size = 32;
}

TextureAtlas::TextureAtlas()
{

}

TextureAtlas::~TextureAtlas()
{

}

void TextureAtlas::LoadAll()
{
    std::vector<std::string> files;
    getFiles(Prefecences::Instance()->getTexturesDir() + "atlas/", files);
    LOG(verbose) << "texatlas found " << files.size() << " files";
    tex.emplace_back();
    AtlasPart &ap = *(--tex.end());
    ap.tex = std::make_shared<Texture>();

    Pixmap atlas(glm::vec2(atlas_dim, atlas_dim));

    int x = 0, y = 0, count = 0;
    auto add_image = [&](Pixmap &pmap, const std::string &file){

        atlas.Blit(pmap, glm::vec2(x, y));

        uvs.push_back(glm::vec4(x, y, x + pmap.width, y + pmap.height)/static_cast<float>(atlas_dim));
        size.push_back({pmap.width, pmap.height});

        refs[file] = glm::ivec2(0, count);

        x += pmap.width;
        if(x >= atlas_dim - sprite_size)
        {
            x = 0;
            y += 32;
        }
        ++count;
    };

    Pixmap err(glm::vec2(error_image.width, error_image.width));
    err.data.insert(std::begin(err.data), std::begin(error_image.pixel_data), std::end(error_image.pixel_data));
    add_image(err, "error");


    for(std::string file: files)
    {
        Pixmap pmap(Prefecences::Instance()->getTexturesDir() + "atlas/" + file);

        add_image(pmap, file.substr(0, file.find_last_of('.')));
    }

    LOG(verbose) << "texatlas load " << count << " pixmaps";

    ap.tex->Load(atlas, false, false);

    LOG(verbose) << "texatlas load texture";
}

std::vector<AtlasPart> TextureAtlas::tex;

std::vector<glm::vec4> TextureAtlas::uvs;
std::vector<glm::vec2> TextureAtlas::size;
std::vector<std::vector<std::vector<bool>>> TextureAtlas::pixels;

std::unordered_map<std::string, glm::ivec2> TextureAtlas::refs;

