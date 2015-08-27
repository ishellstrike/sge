/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef PREFECENCES_H
#define PREFECENCES_H
#include "singleton.h"
#include <glm/glm.hpp>
#include <string>

#define RESX Prefecences::Instance()->resolution.x
#define RESY Prefecences::Instance()->resolution.y
#define RESX_float static_cast<float>(RESX)
#define RESY_float static_cast<float>(RESY)

class Prefecences
{
public:
protected:
    static Prefecences *m_instance;
    Prefecences() {}
    ~Prefecences() {}

public:
    static Prefecences* Instance()
    {
        static std::mutex mutex;
        if(!m_instance)
        {
            mutex.lock();

            if (!m_instance)
                m_instance = new Prefecences();

            mutex.unlock();
        }
        return m_instance;
    }

    static void DeleteInstance()
    {
        static std::mutex mutex;
        if(!m_instance)
        {
            mutex.lock();
            if(!m_instance)
            {
                m_instance = nullptr;
                delete m_instance;
            }
            mutex.unlock();
        }
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

    bool hdr_on = false;
    bool defered_debug = false;
};

#endif // PREFECENCES_H
