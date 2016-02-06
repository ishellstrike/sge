#ifndef PACKETHOLDER_H
#define PACKETHOLDER_H
#include "packet.h"
#include "packetrequestsector.h"
#include "packetresponsesector.h"
#include <boost/serialization/shared_ptr.hpp>

class PacketHolder
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar.template register_type<PacketRequestSector>();
        ar.template register_type<PacketResponseSector>();
        ar.template register_type<Packet>();

        ar & packet;
    }

public:

    std::shared_ptr<Packet> packet;

    template<typename Ty_, typename ... Args>
    void Init(Args && ... args)
    {
        packet = std::shared_ptr<Ty_>( new Ty_(std::forward<Args>(args)...) );
    }

    std::vector<uint8_t> Serialize() const;
    void Deserialize(const std::vector<uint8_t> &d);
};

#endif // PACKETHOLDER_H
