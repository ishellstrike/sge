#ifndef PACKETREQUESTSECTOR_H
#define PACKETREQUESTSECTOR_H
#include "packet.h"
#include "glm/glm.hpp"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>

class PacketRequestSector : public Packet
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::base_object<Packet>(*this);
        ar & pos;
    }

public:
    PacketRequestSector(const glm::ivec2 &p);
    PacketRequestSector();
    glm::ivec2 pos;
};

#endif // PACKETREQUESTSECTOR_H
