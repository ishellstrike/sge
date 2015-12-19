/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "textureatlas.h"
#include "resources/pixmap.h"
#include "prefecences.h"
#include "logger.h"
#include "resources/error_image.h"
#include <boost/filesystem.hpp>

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
    boost::filesystem::path targetDir(Prefecences::Instance()->getTexturesDir() + "atlas/");
    boost::filesystem::recursive_directory_iterator iter(targetDir);

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

        x += sprite_size;
        if(x >= atlas_dim)
        {
            x = 0;
            y += sprite_size;
        }
        ++count;
    };

    Pixmap err(glm::vec2(error_image.width, error_image.width));
    err.data.insert(std::begin(err.data), std::begin(error_image.pixel_data), std::end(error_image.pixel_data));
    add_image(err, "error");

    for(const boost::filesystem::path &file : iter){
        if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".png")
        {
            Pixmap pmap(file.string());

            add_image(pmap, file.stem().string());
        }
    }

    LOG(trace) << "texatlas load " << count << " pixmaps";

    ap.tex->Load(atlas, false, false);

    LOG(trace) << "texatlas load texture";
}

std::vector<AtlasPart> TextureAtlas::tex;

std::vector<glm::vec4> TextureAtlas::uvs;
std::vector<glm::vec2> TextureAtlas::size;
std::vector<std::vector<std::vector<bool>>> TextureAtlas::pixels;

std::unordered_map<std::string, glm::ivec2> TextureAtlas::refs;

