/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey
        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include <GLFW/glfw3.h>
#include <map>
#include <functional>

class Keybind
{
public:
    enum Actions {
        ACT_INVENTORY_MENU,
        ACT_BUILDING_MENU,
        ACT_CRAFTING_MENU,
        ACT_OPTIONS_MENU,
        ACT_MULTIMENU,
        ACT_MAP,
        ACT_DEBUG_MENU,
        ACT_TAKE_ALL
    }

    std::map<int, Actions> binds;
    
    void Deserialize(const rapidjson::Value &val);
    rapidjson::Document Serialize();
    int GetBind(Actions a);
    
    void SaveCurrent();
    void LoadCurrent();
    
    void LoadDefault();

private:
    static const std::string default_file = "default_bindings.json";
    static const std::string current_file = "current_bindings.json";
}
