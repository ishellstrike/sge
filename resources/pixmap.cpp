#include "pixmap.h"
#include "error_image.h"
#include "../lodepng/lodepng.h"
#include "../logger.h"
#include "../helper.h"

/*!
 * \brief Pixmap::Pixmap Выполняет загрузку png в битовую карту
 * \param name Имя изображения *.png
 */
Pixmap::Pixmap(const std::string &name)
{
    unsigned error;
    try
    {
        error = lodepng::decode(data, width, height, name);
    }
    catch (...)
    {

    }

    if(error)
    {
        LOG(info) << "png error " << error << ": " << lodepng_error_text(error) << " for " << name;
        data.clear();
        data.insert(data.end(), &error_image.pixel_data[0], &error_image.pixel_data[error_image.size]);
        width = error_image.width;
        height = error_image.height;
        //name = error_image.name;
    }
}

/*!
 * \brief Pixmap::Pixmap Создает пустую битовую карту заданного размера
 * \param size размер битовой карты
 */
Pixmap::Pixmap(glm::vec2 size)
{
    width = (int) size.x;
    height = (int) size.y;
    data.resize((int)(size.x * size.y) * 4, 0);
}

Pixmap::Pixmap()
{

}

Pixmap::~Pixmap()
{

}

/*!
 * \brief Pixmap::Blit Производит слияние выбранной битовой карты с этой
 * \param source Исходные данные для слияния
 * \param pos Верхняя елвая координата начала слияния
 */
void Pixmap::Blit(const Pixmap &source, const glm::vec2 &pos)
{
    assert(pos.x >= 0 && pos.y >= 0);
    if(width < source.width + pos.x || height < source.height + pos.y) {
        LOG(error) << "Blit error. Target image not enough large. " <<
                      sge::string_format("(%g, %g) to (%g, %g) into (%d, %d)",
                                    pos.x, pos.y,
                                    source.width + pos.x, source.height + pos.y,
                                    width, height);
        throw std::out_of_range("target pixmap not enough large");
    }

    for(unsigned i = 0; i < source.height; ++i)
        memcpy(&data[(height * (i + (int) pos.y) + (int) pos.x)*4],
               &source.data[(source.height * i)*4],
               sizeof(unsigned char) * 4 * source.width);
}

