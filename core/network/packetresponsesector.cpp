#include "packetresponsesector.h"

PacketResponseSector::PacketResponseSector() : Packet(Packet::TidFor<PacketResponseSector>())
{

}

PacketResponseSector::PacketResponseSector(std::shared_ptr<Sector> &__s) : s(__s), Packet(Packet::TidFor<PacketResponseSector>())
{

}

