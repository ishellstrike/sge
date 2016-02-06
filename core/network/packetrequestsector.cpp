#include "packetrequestsector.h"
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

PacketRequestSector::PacketRequestSector(const glm::ivec2 &p) : pos(p)
{

}

PacketRequestSector::PacketRequestSector()
{

}
