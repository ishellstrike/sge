/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef PREFECENCES_H
#define PREFECENCES_H
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "keyconfig.h"

#define RESX Prefecences::Instance()->resolution.x
#define RESY Prefecences::Instance()->resolution.y
#define RESX_float static_cast<float>(RESX)
#define RESY_float static_cast<float>(RESY)
#define sscale Prefecences::Instance()->scale_sprite

class Prefecences
{
public:
protected:
    Prefecences() {}

public:
    float scale_sprite = 32;

    static Prefecences* Instance()
    {
        static auto m_instance = std::unique_ptr<Prefecences>(new Prefecences());
        return m_instance.get();
    }

    /*!
     * \brief current window resolution
     */
    glm::ivec2 resolution = glm::ivec2(800, 800);

    /*!
     * \brief data dir
     */
    std::string data_dir = "data/";

    /*!
     * \brief texture_data_subdir
     */
    std::string texture_data_subdir = "textures/";
    /*!
     * \brief getTexturesDir
     * \return textures directory name
     */
    const std::string getTexturesDir() const
    {
        return data_dir + texture_data_subdir;
    }

    std::string sounds_data_subdir = "sounds/";
    const std::string getSoundsDir() const
    {
        return data_dir + sounds_data_subdir;
    }

    /*!
     * \brief shaders data subdir
     */
    std::string shaders_data_subdir = "shaders/";
    /*!
     * \brief getShadersDir
     * \return shaders directory name
     */
    const std::string getShadersDir() const
    {
        return data_dir + shaders_data_subdir;
    }

    /*!
     * \brief shaders data subdir
     */
    std::string fonts_data_subdir = "fonts/";
    /*!
     * \brief getFontsDir
     * \return fonts directory name
     */
    const std::string getFontsDir() const
    {
        return data_dir + fonts_data_subdir;
    }

    /*!
     * \brief json data subdir
     */
    std::string json_data_subdir = "json/";
    /*!
     * \brief getJsonDir
     * \return json directory name
     */
    const std::string getJsonDir() const
    {
        return data_dir + json_data_subdir;
    }

    bool hdr_on = true;
    bool defered_debug = false;
    bool starnest_on = true;
    float double_c = 0.3f;
    bool no_sound = false;
	std::string server_adress = "127.0.0.1";
	unsigned server_port = 7777;
};

#endif // PREFECENCES_H
