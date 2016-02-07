#ifndef PLAYEREMPLACER_H
#define PLAYEREMPLACER_H
#include <boost/serialization/serialization.hpp>
#include <glm/glm.hpp>

class PlayerEmplacer
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & pos;
        ar & look;
        ar & id;
    }

public:
    PlayerEmplacer();
    PlayerEmplacer( glm::vec3 __pos, float __look, size_t __id);

    glm::vec3 pos;
    float look;
    size_t id;
};

#endif // PLAYEREMPLACER_H
