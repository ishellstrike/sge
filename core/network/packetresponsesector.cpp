#include "packetresponsesector.h"

PacketResponseSector::PacketResponseSector() : Packet(GetId())
{

}

PacketResponseSector::PacketResponseSector(std::shared_ptr<Sector> &__s) : s(__s), Packet(GetId())
{

}

size_t PacketResponseSector::GetId()
{
    return 4;
}

