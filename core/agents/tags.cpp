#include "tags.h"



void Tags::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(tags));
}
