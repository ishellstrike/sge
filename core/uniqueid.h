#ifndef UNIQUEID_H
#define UNIQUEID_H
typedef unsigned long long uniqId;

class UniqueId
{
    static const uniqId type_size = 1L<<32L;

    static uniqId _next_typeId()
    {
        static uniqId uniq_id = 0L;
        ++uniq_id;
        return uniq_id;
    }

    template<typename _Ty>
    static uniqId _get_type_offset()
    {
        static uniqId this_type = _next_typeId();
        return this_type * type_size;
    }

public:
    template<typename _Ty>
    static uniqId next()
    {
        static uniqId uniq_id = 0L;
        ++uniq_id;
        return _get_type_offset<_Ty>() + uniq_id;
    }

    template<typename _Ty>
    static bool is(uniqId id)
    {
        if(id == 0) return false;
        return id / type_size == _get_type_offset<_Ty>();
    }

    UniqueId() = delete;
};

#endif // UNIQUEID_H
