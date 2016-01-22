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
    std::map<int, std::function<void()>> binds;
    
    void Deserialize(const rapidjson::Value &val);
    rapidjson::Document Serialize();
    
    void SaveCurrent();
    void LoadCurrent();
    
    void SaveDefault();
    void LoadDefault();

private:
    static const std::string default_file = "default_bindings.json";
    static const std::string current_file = "current_bindings.json";
}
