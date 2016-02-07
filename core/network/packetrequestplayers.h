#ifndef PACKETREQUESTPLAYERS_H
#define PACKETREQUESTPLAYERS_H
#include "packet.h"
#include "glm/glm.hpp"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>

class PacketRequestPlayers : public Packet
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::base_object<Packet>(*this);
        ar & our_pos;
        ar & our_phi;
        ar & id;
    }

public:
    PacketRequestPlayers();
    PacketRequestPlayers(const glm::vec3 & pos, float phi, size_t i);
    size_t GetId();

    glm::vec3 our_pos;
    float our_phi;
    size_t id;
};

#endif // PACKETREQUESTPLAYERS_H
