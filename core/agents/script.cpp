#include "script.h"



void Script::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(source), NVP(path));
}
