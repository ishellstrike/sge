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

TextureAtlas::TextureAtlas()
{

}

TextureAtlas::~TextureAtlas()
{

}

glm::vec2 TextureAtlas::GetUV(int n)
{
    static int inrow = 2048/64;
    int row = n % inrow;
    int column = n / inrow;
    return glm::vec2(2048.f/(row*64.f), 2048.f/(column*32.f));
}

void TextureAtlas::LoadAll()
{
    std::vector<std::string> files;
    getFiles(Prefecences::Instance()->getAtlasDir(), files);
    LOG(verbose) << "texatlas found " << files.size() << " files";

    Pixmap atlas(glm::vec2(2048, 2048));

    int x = 0, y = 0, i = 0;
    for(std::string file: files)
    {
        Pixmap tex(Prefecences::Instance()->getAtlasDir() + file);
        atlas.Blit(tex, glm::vec2(x * 64, y * 32));
        refs[file] = i;
        x++;
        if(x >= 2048/64)
        {
            x = 0;
            y++;
        }
        ++i;
    }
    LOG(verbose) << "texatlas load " << i << " pixmaps";

    tex->Load(atlas, false, false);
    tex->name = "texture atlas 1";
    LOG(verbose) << "texatlas load texture";
}

std::shared_ptr<Texture> TextureAtlas::tex = std::make_shared<Texture>();
std::unordered_map<std::string, int> TextureAtlas::refs;

