#include "pixmap.h"
#include "lodepng/lodepng.h"
#include "logger.h"
#include "helper.inl"

Pixmap::Pixmap(const std::string &a)
{
    auto error = lodepng::decode(data, width, height, a);
    if(error) LOG(info) << "png error " << error << ": " << lodepng_error_text(error) << " for " << a;
}

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

void Pixmap::Blit(const Pixmap &source, const glm::vec2 &pos)
{
    assert(pos.x >= 0 && pos.y >= 0);
    if(width < source.width + pos.x || height < source.height + pos.y) {
        LOG(error) << "Blit error. Target image not enough large. " <<
                      string_format("(%g, %g) to (%g, %g) into (%d, %d)",
                                    pos.x, pos.y,
                                    source.width + pos.x, source.height + pos.y,
                                    width, height);
        throw std::out_of_range("target pixmap not enough large");
    }

    for(unsigned i = 0; i < source.height; i++)
        memcpy(&data[(height * (i + (int) pos.y) + (int) pos.x)*4],
               &source.data[(source.height * i)*4],
               sizeof(unsigned char) * 4 * source.width);
}

