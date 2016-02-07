#include "packetresponseplayers.h"

PacketResponsePlayers::PacketResponsePlayers() : Packet(GetId())
{

}

PacketResponsePlayers::PacketResponsePlayers(const std::vector<PlayerEmplacer> &e) : Packet(GetId()), emp(e)
{

}

size_t PacketResponsePlayers::GetId()
{
    return 3;
}

