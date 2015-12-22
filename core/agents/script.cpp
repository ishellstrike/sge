#include "script.h"



void Script::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(source), NVP(path));
}
