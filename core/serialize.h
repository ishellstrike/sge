/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SERIALIZE_H
#define SERIALIZE_H
#include "rapidjson/document.h"
#include <string>
#include <sstream>
#include <functional>
#include "helper.h"

struct DeserializeHelper {
    template <class T>
    inline static std::pair<const char *, T> make_nvp(const char *name, T &&value) {
        return{name, std::forward<T>(value)};
    }

#define NVP(T) DeserializeHelper::make_nvp(#T, T)
#define DESERIALIZE(...) DeserializeHelper::deserialize(val, __VA_ARGS__)

    static void deserialize(const rapidjson::Value &val)
    {
        (void)val;
    }

    template <typename Last>
    static void deserialize(const rapidjson::Value &val, const Last &last)
    {
        __deserialize(val, last.first, last.second);
    }

    template <typename First, typename... Rest>
    static void deserialize(const rapidjson::Value &val, const First &first, const Rest&... rest)
    {
        __deserialize(val, first.first, first.second);
        deserialize(val, rest...);
    }


    static void serialize(rapidjson::Document &doc)
    {
        (void)doc;
    }

    template <typename Last>
    static void serialize(rapidjson::Document &doc)
    {
        __serialize(doc, last.first, last.second);
    }

    template <typename First, typename... Rest>
    static void serialize(rapidjson::Document &doc)
    {
        __serialize(doc, first.first, first.second);
        serialize(doc, rest...);
    }

private:

    template<typename _Ty>
    static void __deserialize(const rapidjson::Value &val, const char *, _Ty &target)
    {
        target.Deserialize(val);
    }

    template<typename _Ty>
    static void __deserialize(const rapidjson::Value &val, const char *s, std::vector<_Ty> &target)
    {

        if(val.HasMember(s) && val[s].IsArray())
        {
            const rapidjson::Value &arr = val[s];
            for(decltype(arr.Size()) i = 0; i < arr.Size(); i++)
            {
                _Ty part;
                __deserialize(arr[i], "", part);
                target.push_back(std::move(part));
            }
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, int &target)
    {
        if(val.HasMember(s))
        {
            if(!val[s].IsInt())
                throw std::invalid_argument(sge::string_format("value %s is not a integer", s));
            target = val[s].GetInt();
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, std::string &target)
    {
        if(val.HasMember(s))
        {
            if(!val[s].IsString())
                throw std::invalid_argument(sge::string_format("value %s is not a string", s));
            target = val[s].GetString();
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, float &target)
    {
        if(val.HasMember(s))
        {
            if(!val[s].IsNumber())
                throw std::invalid_argument(sge::string_format("value %s is not a number", s));
            target = static_cast<float>(val[s].GetDouble());
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, bool &target)
    {
        if(val.HasMember(s))
        {
            if(!val[s].IsBool_())
                throw std::invalid_argument(sge::string_format("value %s is not a bool", s));
            target = val[s].GetBool_();
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, glm::vec2 &target)
    {
        if(val.HasMember(s) && val[s].IsArray())
        {
            const rapidjson::Value &arr = val[s];
            if(arr.Size() != 2)          throw std::invalid_argument(sge::string_format("value %s is not vec2", s));
            if(!arr.Begin()->IsNumber()) throw std::invalid_argument(sge::string_format("value %s[0] is not a number", s));
            if(!arr[1].IsNumber())       throw std::invalid_argument(sge::string_format("value %s[1] is not a number", s));

            target.x = static_cast<float>(arr.Begin()->GetDouble());
            target.y = static_cast<float>(arr[1].GetDouble());
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, glm::vec3 &target)
    {
        if(val.HasMember(s) && val[s].IsArray())
        {
            const rapidjson::Value &arr = val[s];
            if(arr.Size() != 3)          throw std::invalid_argument("value is not vec3");
            if(!arr.Begin()->IsNumber()) throw std::invalid_argument(sge::string_format("value %s[0] is not a number", s));
            if(!arr[1].IsNumber())       throw std::invalid_argument(sge::string_format("value %s[1] is not a number", s));
            if(!arr[2].IsNumber())       throw std::invalid_argument(sge::string_format("value %s[2] is not a number", s));

            target.x = static_cast<float>(arr.Begin()->GetDouble());
            target.y = static_cast<float>(arr[1].GetDouble());
            target.z = static_cast<float>(arr[2].GetDouble());
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, glm::vec4 &target)
    {
        if(val.HasMember(s) && val[s].IsArray())
        {
            const rapidjson::Value &arr = val[s];
            if(arr.Size() != 4)          throw std::invalid_argument("target variable is not vec4");
            if(!arr.Begin()->IsNumber()) throw std::invalid_argument(sge::string_format("value %s[0] is not a number", s));
            if(!arr[1].IsNumber())       throw std::invalid_argument(sge::string_format("value %s[1] is not a number", s));
            if(!arr[2].IsNumber())       throw std::invalid_argument(sge::string_format("value %s[2] is not a number", s));
            if(!arr[3].IsNumber())       throw std::invalid_argument(sge::string_format("value %s[3] is not a number", s));

            target.x = static_cast<float>(arr.Begin()->GetDouble());
            target.y = static_cast<float>(arr[1].GetDouble());
            target.z = static_cast<float>(arr[2].GetDouble());
            target.w = static_cast<float>(arr[3].GetDouble());
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, std::vector<int> &target)
    {
        if(val.HasMember(s))
        {
            const rapidjson::Value &arr = val[s];
            for(decltype(arr.Size()) i = 0; i < arr.Size(); i++)
            {
                if(!arr[i].IsInt())
                    throw std::invalid_argument(sge::string_format("value %s[%d] is not a integer", s, i));
                target.push_back(arr[i].GetInt());
            }
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, std::vector<std::string> &target)
    {
        if(val.HasMember(s) && val[s].IsArray())
        {
            const rapidjson::Value &arr = val[s];
            for(decltype(arr.Size()) i = 0; i < arr.Size(); i++)
            {
                if(!arr[i].IsString())
                    throw std::invalid_argument(sge::string_format("value %s[%d] is not a string", s, i));
                target.push_back(arr[i].GetString());
            }
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, std::vector<bool> &target)
    {
        if(val.HasMember(s) && val[s].IsArray())
        {
            const rapidjson::Value &arr = val[s];
            for(decltype(arr.Size()) i = 0; i < arr.Size(); i++)
            {
                if(!arr[i].IsBool_())
                    throw std::invalid_argument(sge::string_format("value %s[%d] is not a bool", s, i));
                target.push_back(arr[i].GetBool_());
            }
        }
    }

    template<>
    static void __deserialize(const rapidjson::Value &val, const char *s, std::vector<float> &target)
    {
        if(val.HasMember(s) && val[s].IsArray())
        {
            const rapidjson::Value &arr = val[s];
            for(decltype(arr.Size()) i = 0; i < arr.Size(); i++)
            {
                if(!arr[i].IsDouble())
                    throw std::invalid_argument(sge::string_format("value %s[%d] is not a number", s, i));
                target.push_back(static_cast<float>(arr[i].GetDouble()));
            }
        }
    }

    template<typename _Ty>
    static void __serialize(rapidjson::Document &doc, const char *, const _Ty &target)
    {
        target.Serialize(val);
    }

    template<typename _Ty>
    static void __serialize(rapidjson::Document &doc, const char *s, const std::vector<_Ty> &target)
    {
        const rapidjson::Value &arr;
        arr.SetArray();

        for(decltype(target.Size()) i = 0; i < target.Size(); i++)
        {
            rapidjson::Value tv;
            __serialize(tv, "", target[i]);

            arr.PushBack(tv, doc.GetAllocator());
        }

        doc[s] = arr;
    }
};

#endif // SERIALIZE_H

