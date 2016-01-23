#include "keyconfig.h"
#include "boost/assign.hpp"

void Keybind::Deserialize(const rapidjson::Value &val)
{

}

rapidjson::Document Keybind::Serialize()
{
    return rapidjson::Document();
}

int Keybind::GetBind(int a)
{
    return binds[a];
}

void Keybind::SaveCurrent()
{

}

void Keybind::LoadCurrent()
{

}

void Keybind::SetDefault()
{
    binds.clear();
    binds = std::map<int, int>
            {{ACT_GO_UP,         GLFW_KEY_W},
            {ACT_GO_DOWN,        GLFW_KEY_S},
            {ACT_GO_LEFT,        GLFW_KEY_A},
            {ACT_GO_RIGHT,       GLFW_KEY_D},
            {ACT_CLOSE_TOP,      GLFW_KEY_ESCAPE},
            {ACT_INVENTORY_MENU, GLFW_KEY_I},
            {ACT_BUILDING_MENU,  GLFW_KEY_B},
            {ACT_CRAFTING_MENU,  GLFW_KEY_C},
            {ACT_OPTIONS_MENU,   GLFW_KEY_O},
            {ACT_MULTIMENU,      GLFW_KEY_P},
            {ACT_MAP,            GLFW_KEY_M},
            {ACT_DEBUG_MENU,     GLFW_KEY_GRAVE_ACCENT},
            {ACT_TAKE_ALL,       GLFW_KEY_R}};
}

std::map<int, int> Keybind::binds;
