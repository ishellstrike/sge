#ifndef PLAYER_H
#define PLAYER_H
#include "core/serialize.h"

class Player
{
public:
    Player();

    enum Difficulty
    {
        diff_casual,
        diff_normal,
        diff_roguelike
    } difficulty = diff_normal;

    enum WeightSettings
    {
        weight_normal,
        weight_no_weight
    } weightSettings = weight_normal;

    std::string name, surname;
    int age;

    enum Gender
    {
        male,
        female
    } gender = male;
};

#endif // PLAYER_H
