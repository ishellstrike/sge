#ifndef PACKETRESPONSESECTOR_H
#define PACKETRESPONSESECTOR_H
#include <memory>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "packet.h"
#include "core/sector.h"

class PacketResponseSector : public Packet
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::base_object<Packet>(*this);
        ar & s;
    }

public:
    PacketResponseSector();
    PacketResponseSector(std::shared_ptr<Sector> &__s);
    size_t GetId();

    std::shared_ptr<Sector> s;
};

#endif // PACKETRESPONSESECTOR_H
