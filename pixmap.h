#ifndef PIXMAP_H
#define PIXMAP_H
#include <vector>
#include <glm/glm.hpp>
#include <string>

struct Pixmap
{
    Pixmap(const std::string &a);
    Pixmap(glm::vec2 size);
    Pixmap();
    ~Pixmap();
    std::vector<unsigned char> data;
    unsigned int width = 0, height = 0;

    void Blit(const Pixmap &source, const glm::vec2 &pos);
};

#endif // PIXMAP_H
