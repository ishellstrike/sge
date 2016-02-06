#ifndef PACKET_H
#define PACKET_H
#include <vector>
#include <inttypes.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <memory>
#include <glm/glm.hpp>

namespace boost
{
namespace serialization
{
template<class Archive>
void serialize(Archive & ar, glm::ivec2& vec, const unsigned int)
{
    ar & boost::serialization::make_nvp("x", vec.x);
    ar & boost::serialization::make_nvp("y", vec.y);
}
}
}

class Packet
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar & id;
    }

    static size_t NextTid()
    {
        static size_t next_id(0);
        return next_id++;
    }

public:
    virtual ~Packet(){}
    Packet(size_t __id = TidFor<Packet>()) : id(__id)
    {

    }

    size_t id;

    template <typename T_>
    static size_t TidFor()
    {
        static size_t result(NextTid());
        return result;
    }
};

#endif // PACKET_H
