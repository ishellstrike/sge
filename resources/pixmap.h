/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef PIXMAP_H
#define PIXMAP_H
#include <vector>
#include <glm/glm.hpp>
#include <string>

/*!
 * \brief The Pixmap struct
 *
 * Хранит битовую карту, может быть загружена в @Texture
 */
struct Pixmap
{
    Pixmap(const std::string &name);
    Pixmap(glm::vec2 size);
    Pixmap();
    ~Pixmap();
    std::vector<unsigned char> data;
    unsigned int width = 0, height = 0;

    void Blit(const Pixmap &source, const glm::vec2 &pos);
};

#endif // PIXMAP_H
