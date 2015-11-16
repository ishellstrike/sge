#ifndef UNIQUEID_H
#define UNIQUEID_H
#include <functional>
typedef unsigned uniqId;

class UniqueId
{
    static const uniqId type_size = 1<<20;

    static uniqId _next_typeId()
    {
        static uniqId uniq_id = 0;
        ++uniq_id;
        return uniq_id;
    }

    template<typename _Ty>
    static uniqId _get_type_offset()
    {
        return getTypeId<_Ty>() * type_size;
    }

public:
    template<typename _Ty>
    static uniqId next()
    {
        static uniqId uniq_id = 0;
        ++uniq_id;
        return _get_type_offset<_Ty>() + uniq_id;
    }

    template<typename _Ty>
    static bool is(uniqId id)
    {
        if(id == 0) return false;
        return id / type_size == _get_type_offset<_Ty>();
    }

    template<typename _Ty>
    static uniqId getTypeId()
    {

        static uniqId this_type = []() -> uniqId
        {
                std::hash<std::string> s;
                return s(typeid(_Ty).name());
        }();

        return this_type;
    }

    template<typename _Ty>
    static uniqId getTypeId(const _Ty &val)
    {

        static uniqId this_type = [&]() -> uniqId
        {
                std::hash<std::string> s;
                return s(typeid(val).name());
        }();

        return this_type;
    }

    UniqueId() = delete;
};

#endif // UNIQUEID_H
