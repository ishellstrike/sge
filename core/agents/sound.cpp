#include "sound.h"



void Sound::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(name));
}
