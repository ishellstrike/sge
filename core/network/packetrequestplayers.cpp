#include "packetrequestplayers.h"

PacketRequestPlayers::PacketRequestPlayers() : Packet(GetId())
{

}

PacketRequestPlayers::PacketRequestPlayers(const glm::vec3 &pos, float phi, size_t i) :
    Packet(GetId()),
    our_phi(phi),
    our_pos(pos),
    id(i)
{

}

size_t PacketRequestPlayers::GetId()
{
    return 1;
}


