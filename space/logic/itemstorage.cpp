#include "itemstorage.h"

ItemStorage::ItemStorage()
{

}

multiitem ItemStorage::pop_by_type(const std::string &s)
{
    for(auto d = data.begin(); d != data.end(); ++d)
    {
        if(std::get<0>(*d)->id == s)
        {
            data.erase(d);
            return *d;
        }
    }

    return multiitem();
}

multiitem ItemStorage::pick_by_type(const std::string &s)
{
    for(multiitem &d : data)
    {
        if(std::get<0>(d)->id == s)
        {
            return d;
        }
    }

    return multiitem();
}

void ItemStorage::push(std::shared_ptr<Item> &item)
{
    push(std::make_tuple(item, 1));
}

void ItemStorage::push(multiitem &item)
{
    auto &d = pick_by_type(std::get<0>(item)->id);
    if(std::get<0>(d) != 0)
    {
        std::get<1>(d) += std::get<1>(item);
        return;
    }

    data.push_back(item);
}

