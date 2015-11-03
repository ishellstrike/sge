#ifndef UNIQUEID_H
#define UNIQUEID_H
#include <functional>
typedef unsigned long long uniqId;

class UniqueId
{
    static const uniqId type_size = 1ul<<32ul;

    static uniqId _next_typeId()
    {
        static uniqId uniq_id = 0ul;
        ++uniq_id;
        return uniq_id;
    }

    template<typename _Ty>
    static uniqId _get_type_id()
    {

        static uniqId this_type = []() -> uniqId
        {
                std::hash<std::string> s;
                return s(typeid(_Ty).name());
        }();

        return this_type;
    }

    template<typename _Ty>
    static uniqId _get_type_offset()
    {
        return _get_type_id<_Ty>() * type_size;
    }

public:
    template<typename _Ty>
    static uniqId next()
    {
        static uniqId uniq_id = 0ul;
        ++uniq_id;
        return _get_type_offset<_Ty>() + uniq_id;
    }

    template<typename _Ty>
    static bool is(uniqId id)
    {
        if(id == 0ul) return false;
        return id / type_size == _get_type_offset<_Ty>();
    }

    UniqueId() = delete;
};

#endif // UNIQUEID_H
