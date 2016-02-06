#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/exception_ptr.hpp>
#include <sstream>

#include "logger.h"
#include "packetholder.h"

std::vector<uint8_t> PacketHolder::Serialize() const
{
    std::stringstream ss;
    boost::archive::text_oarchive oa(ss);

    oa.register_type<PacketRequestSector>();
    oa.register_type<PacketResponseSector>();
    oa.register_type<Packet>();

    oa << packet;

    std::vector< uint8_t > request;
    std::string s = ss.str();
    std::copy(s.begin(), s.end(), std::back_inserter(request));
    return request;
}

void PacketHolder::Deserialize(const std::vector<uint8_t> &d)
{
    std::stringstream ss;
    ss << std::string(d.begin(), d.end());

    boost::archive::text_iarchive ia(ss);

    ia.register_type<PacketRequestSector>();
    ia.register_type<PacketResponseSector>();
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

