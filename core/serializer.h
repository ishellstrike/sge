#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "serialize.h"

struct SerializeHelper {
    static void serialize(rapidjson::Document &doc, rapidjson::Value &v)
    {
        (void)doc;
        (void)v;
    }

    template <typename Last>
    static void serialize(rapidjson::Document &doc, rapidjson::Value &v, const Last &last)
    {
        __serialize(doc, v, last.first, last.second);
    }

    template <typename First, typename... Rest>
    static void serialize(rapidjson::Document &doc, rapidjson::Value &v, const First &first, const Rest&... rest)
    {
        __serialize(doc, v, first.first, first.second);
        serialize(doc, rest...);
    }

private:
    template<typename _Ty>
    static void __serialize(rapidjson::Document &doc, rapidjson::Value &v, const char *, const _Ty &target)
    {
        target.Serialize(doc, v);
    }

    template<>
    static void __serialize(rapidjson::Document &doc, rapidjson::Value &v, const char *s, const int &target)
    {
        v.AddMember(s, target, doc.GetAllocator());
    }

    template<>
    static void __serialize<char *>(rapidjson::Document &doc, rapidjson::Value &v, const char *s, char * const & target)
    {
        v.AddMember(s, &target, doc.GetAllocator());
    }

    template<>
    static void __serialize(rapidjson::Document &doc, rapidjson::Value &v, const char *s, const float &target)
    {
        v.AddMember(s, target, doc.GetAllocator());
    }

    template<>
    static void __serialize(rapidjson::Document &doc, rapidjson::Value &v, const char *s, const bool &target)
    {
        v.AddMember(s, target, doc.GetAllocator());
    }

    template<typename _Ty>
    static void __serialize(rapidjson::Document &doc, rapidjson::Value &v, const char *s, const std::vector<_Ty> &target)
    {
        const rapidjson::Value &arr;
        arr.SetArray();

        for(decltype(target.Size()) i = 0; i < target.Size(); i++)
        {
            rapidjson::Value tv;
            __serialize(tv, "", target[i]);

            arr.PushBack(tv, doc.GetAllocator());
        }

        v.AddMember(s, arr, doc.GetAllocator());
    }
};

#endif // SERIALIZER_H
