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

void TextureAtlas::LoadAll()
{
    std::vector<std::string> files;
    getFiles(Prefecences::Instance()->getTexturesDir() + "atlas/", files);
    LOG(verbose) << "texatlas found " << files.size() << " files";
    tex.emplace_back();
    AtlasPart &ap = *(--tex.end());
    ap.tex = std::make_shared<Texture>();
    ap.tex_n = std::make_shared<Texture>();
    ap.tex_o = std::make_shared<Texture>();

    Pixmap atlas(glm::vec2(2048, 2048));
    Pixmap atlas_o(glm::vec2(2048, 2048));

    int x = 0, y = 0, count = 0, max_y = 0;
    for(std::string file: files)
    {
        Pixmap tex(Prefecences::Instance()->getTexturesDir() + "atlas/" + file);
        x += tex.width;
        max_y = max(max_y, tex.height);

        Pixmap tex_o(glm::vec2(tex.width, tex.height));
        auto data = std::vector<std::vector<bool>>(tex.width);
        for(int i = 0; i < tex.width; i++)
        {
            data[i] = std::vector<bool>(tex.height);
            for(int j = 0; j < tex.height; j++)
            {
                data[i][j] = tex.data[(tex.height*j + i)*4    ] != 0 &&
                             tex.data[(tex.height*j + i)*4 + 1] != 0 &&
                             tex.data[(tex.height*j + i)*4 + 2] != 0 &&
                             tex.data[(tex.height*j + i)*4 + 3] != 0;

                if(data[i][j])
                {
                    if(i < tex.width - 1)
                    {
                        tex_o.data[(tex_o.height * j + i + 1)*4    ] = 255;
                        tex_o.data[(tex_o.height * j + i + 1)*4 + 1] = 255;
                        tex_o.data[(tex_o.height * j + i + 1)*4 + 2] = 255;
                        tex_o.data[(tex_o.height * j + i + 1)*4 + 3] = 255;
                    }

                    if(j < tex.height - 1)
                    {
                        tex_o.data[(tex_o.height * (j + 1) + i)*4    ] = 255;
                        tex_o.data[(tex_o.height * (j + 1) + i)*4 + 1] = 255;
                        tex_o.data[(tex_o.height * (j + 1) + i)*4 + 2] = 255;
                        tex_o.data[(tex_o.height * (j + 1) + i)*4 + 3] = 255;
                    }

                    if(i > 0)
                    {
                        tex_o.data[(tex_o.height * j + i - 1)*4    ] = 255;
                        tex_o.data[(tex_o.height * j + i - 1)*4 + 1] = 255;
                        tex_o.data[(tex_o.height * j + i - 1)*4 + 2] = 255;
                        tex_o.data[(tex_o.height * j + i - 1)*4 + 3] = 255;
                    }

                    if(j > 0)
                    {
                        tex_o.data[(tex_o.height * (j - 1) + i)*4    ] = 255;
                        tex_o.data[(tex_o.height * (j - 1) + i)*4 + 1] = 255;
                        tex_o.data[(tex_o.height * (j - 1) + i)*4 + 2] = 255;
                        tex_o.data[(tex_o.height * (j - 1) + i)*4 + 3] = 255;
                    }
                }
            }
        }

        atlas_o.Blit(tex_o, glm::vec2(x, y));
        atlas.Blit(tex, glm::vec2(x, y));

        pixels.push_back(data);
        uvs.push_back(glm::vec4(x, y, x + tex.width, y + tex.height)/2048.f);
        size.push_back({tex.width, tex.height});

        refs[file] = glm::ivec2(0, count);

        if(x >= 2048)
        {
            x = 0;
            y+= max_y;
        }
        ++count;
    }

    getFiles(Prefecences::Instance()->getTexturesDir() + "normal/", files);

    Pixmap atlas_n(glm::vec2(2048, 2048));

    x = 0; y = 0; count = 0; max_y = 0;
    for(std::string file: files)
    {
        Pixmap tex(Prefecences::Instance()->getTexturesDir() + "normal/" + file);
        x += tex.width;
        max_y = max(max_y, tex.height);

        atlas_n.Blit(tex, glm::vec2(x, y));

        if(x >= 2048)
        {
            x = 0;
            y+= max_y;
        }
        ++count;
    }

    LOG(verbose) << "texatlas load " << count << " pixmaps";

    ap.tex_n->Load(atlas_n, false, false);
    ap.tex_o->Load(atlas_o, false, false);
    ap.tex->Load(atlas, false, false);

    LOG(verbose) << "texatlas load texture";
}

std::vector<AtlasPart> TextureAtlas::tex;

std::vector<glm::vec4> TextureAtlas::uvs;
std::vector<glm::vec2> TextureAtlas::size;
std::vector<std::vector<std::vector<bool>>> TextureAtlas::pixels;

std::unordered_map<std::string, glm::ivec2> TextureAtlas::refs;

