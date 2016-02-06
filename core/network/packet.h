#ifndef PACKET_H
#define PACKET_H
#include <vector>
#include <inttypes.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>

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
    void serialize(Archive &, const unsigned int)
    {
    }

public:
    inline static std::vector<uint8_t> Serealize(const Packet &p)
    {
        std::stringstream ss;
        boost::archive::text_oarchive oa(ss);
        oa << p;

        std::vector< uint8_t > request;
        std::string s = ss.str();
        std::copy(s.begin(), s.end(), std::back_inserter(request));
        return std::move(request);
    }

    inline static std::unique_ptr<Packet> Deserialize(const std::vector<uint8_t> &d)
    {
        std::unique_ptr<Packet> p;
        std::stringstream ss;
        ss << std::string(d.begin(), d.end());

        boost::archive::text_iarchive ia(ss);
        ia >> *p;
        return std::move(p);
    }
};

#endif // PACKET_H
