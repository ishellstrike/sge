#ifndef PREFECENCES_H
#define PREFECENCES_H
#include "singleton.h"
#include <glm/glm.hpp>
#include <string>

#define RESX (static_cast<int>(Prefecences::Instance()->resolution.x))
#define RESY (static_cast<int>(Prefecences::Instance()->resolution.y))

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
    glm::vec2 resolution = glm::vec2(800, 600);

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
};

#endif // PREFECENCES_H
