#include "tags.h"



void Tags::Deserialize(rapidjson::Value &val)
{
    DESERIALIZE(NVP(tags));
}
