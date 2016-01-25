#include "recipe.h"
#include "core/agents/chest.h"

void Recipe::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(name), NVP(descr), NVP(recipe), NVP(result), NVP(subcategory), NVP(category));
}

void Recipe::IsReady(const Chest &chest) const
{

}

void Recipe::Craft(const Chest &chest) const
{

}


void RecipePart::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(value), NVP(level), NVP(count));
    if(!val.HasMember("type"))
        throw std::logic_error("recipe part has no type");
    type = type_caster[val["type"].GetString()];
}

std::map<std::string, RecipePart::Type> RecipePart::type_caster = {{"item",     RecipePart::Item},
                                                                   {"quality",  RecipePart::Quality},
                                                                   {"material", RecipePart::Material},
                                                                   {"tool",     RecipePart::Tool}};


void ResultPart::Deserialize(const rapidjson::Value &val)
{
    DESERIALIZE(NVP(id), NVP(count));
}
