#ifndef PLAYER_H
#define PLAYER_H
#include "core/serialize.h"

class Player
{
public:
    Player();

    enum Difficulty
    {
        casual,
        normal,
        roguelike
    } difficulty = normal;

    enum WeightSettings
    {
        normal,
        no_weight
    } weightSettings = normal;

    std::string name, surname;
    int age;

    enum Gender
    {
        male,
        female
    } gender = male;
};

#endif // PLAYER_H
