/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey
        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
#include <GLFW/glfw3.h>
#include <map>
#include <functional>
#include "rapidjson/document.h"

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
        ACT_TAKE_ALL = 7,

        ACT_CLOSE_TOP = 40,

        ACT_GO_UP = 50,
        ACT_GO_DOWN = 51,
        ACT_GO_RIGHT = 52,
        ACT_GO_LEFT = 53,
    };

    static std::map<int, int> binds;
    
    static void Deserialize(const rapidjson::Value &val);
    static rapidjson::Document Serialize();
    static int GetBind(int a);
    
    static void SaveCurrent();
    static void LoadCurrent();

    static void SetDefault();
private:
    const std::string current_file = "current_bindings.json";
};
