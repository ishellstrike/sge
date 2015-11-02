#ifndef SECTOR_H
#define SECTOR_H
#include <array>
#include <memory>
constexpr int RX = 32;
constexpr int RY = 32;
constexpr int RZ = 32;
constexpr int RXZY = RX*RY*RZ;

class Block
{

};

class Sector
{
public:
   Sector();
   std::array<std::array<std::array<std::unique_ptr<Block>, RZ>, RY> blocks;
};

#endif // SECTOR_H
