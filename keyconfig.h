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
        ACT_INVENTORY_MENU = 0,
        ACT_BUILDING_MENU = 1,
        ACT_CRAFTING_MENU = 2,
        ACT_OPTIONS_MENU = 3,
        ACT_MULTIMENU = 4,
        ACT_MAP = 5,
        ACT_DEBUG_MENU = 6,
        ACT_TAKE_ALL = 7
    }

    std::map<Actions, int> binds;
    
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
