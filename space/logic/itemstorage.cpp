#include "itemstorage.h"

ItemStorage::ItemStorage()
{

}

multiitem ItemStorage::pop_by_type(const std::string &s)
{
    for(auto &d : data)
    {
        if((*d)->id == s)
        {
            data.erase(d);
            return *d;
        }
    }

    return multiitem();
}

multiitem ItemStorage::pick_by_type(const std::string &s)
{
    for(auto &d : data)
    {
        if((*d)->id == s)
        {
            return *d;
        }
    }

    return multiitem();
}

void ItemStorage::push(std::shared_ptr<Item> &item)
{
    push(std::tuple<item, 1>);
}

void ItemStorage::push(multiitem &item)
{
    auto &d = pick_by_type(item.first->id);
    if(d.second != 0)
    {
        d.second += item.second;
        return;
    }

    data.push_back(item);
}

