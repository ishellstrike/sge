#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/exception_ptr.hpp>
#include <sstream>

#include "packetholder.h"

std::vector<uint8_t> PacketHolder::Serialize() const
{
    std::stringstream ss;
    boost::archive::text_oarchive oa(ss);

    oa.register_type<PacketRequestSector>();
    oa.register_type<PacketResponseSector>();
    oa.register_type<PacketRequestPlayers>();
    oa.register_type<PacketResponsePlayers>();
    oa.register_type<Packet>();

    oa << packet;

    std::vector< uint8_t > request(6);
    std::string s = ss.str();

    size_t len = s.size();
    request[5] = 0x0D;
    request[4] = 0x0A;
    request[3] = (len>>24) & 0xFF;
    request[2] = (len>>16) & 0xFF;
    request[1] = (len>>8) & 0xFF;
    request[0] = len & 0xFF;

    std::copy(s.begin(), s.end(), std::back_inserter(request));
    return request;
}

void PacketHolder::Deserialize(const std::vector<uint8_t> &d)
{
    std::stringstream ss;
    ss << std::string(d.begin()+6, d.end());

    boost::archive::text_iarchive ia(ss);

    ia.register_type<PacketRequestSector>();
    ia.register_type<PacketResponseSector>();
    ia.register_type<PacketRequestPlayers>();
    ia.register_type<PacketResponsePlayers>();
    ia.register_type<Packet>();

    try
    {
        ia >> packet;
    }
    catch(boost::exception &e)
    {
        LOG(error) << boost::diagnostic_information(e);
    }
}

