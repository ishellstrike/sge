#ifndef UNIQUEID_H
#define UNIQUEID_H
#include <functional>
typedef unsigned Uid;

class UniqueId
{
    static const Uid type_size = 1<<20;

    static Uid _next_typeId()
    {
        static Uid uniq_id = 0;
        ++uniq_id;
        return uniq_id;
    }

    template<typename _Ty>
    static Uid _get_type_offset()
    {
        return getTypeId<_Ty>() * type_size;
    }

public:
    template<typename _Ty>
    static Uid next()
    {
        static Uid uniq_id = 0;
        ++uniq_id;
        return _get_type_offset<_Ty>() + uniq_id;
    }

    template<typename _Ty>
    static bool is(Uid id)
    {
        if(id == 0) return false;
        return id / type_size == _get_type_offset<_Ty>();
    }

    template<typename _Ty>
    static Uid getTypeId()
    {

        static Uid this_type = []() -> Uid
        {
                std::hash<std::string> s;
                return s(typeid(_Ty).name());
        }();

        return this_type;
    }

    template<typename _Ty>
    static Uid getTypeId(const _Ty &val)
    {

        static Uid this_type = [&]() -> Uid
        {
                std::hash<std::string> s;
                return s(typeid(val).name());
        }();

        return this_type;
    }

    UniqueId() = delete;
};

#endif // UNIQUEID_H
