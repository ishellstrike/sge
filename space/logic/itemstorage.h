#ifndef ITEMSTORAGE_H
#define ITEMSTORAGE_H
#include "item.h"
#include <tuple>
#include <list>
#include <memory>
#include <vector>


typedef std::tuple<std::shared_ptr<Item>, int> multiitem;

class ItemStorage
{
public:
    ItemStorage();

    std::list<multiitem> data;

    multiitem pop_by_type(const std::string &s);

    void push(std::shared_ptr<Item> &item);
    void push(multiitem &item);

    multiitem pick_by_type(const std::string &s);
};

#endif // ITEMSTORAGE_H
