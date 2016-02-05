#ifndef TREE_H__
#define TREE_H__
#include <memory>

template<typename _Ty>
struct Node
{
    _Ty *data;
    Node(_Ty *d) : data(d) {}
    Node() : data(nullptr) {}

    typedef std::shared_ptr<Node<_Ty>> N_ptr;
    bool expanded = false;

    N_ptr sister, child;

    bool if_leaf()
    {
        return !child;
    }

    N_ptr add_child(_Ty *data)
    {
        N_ptr t;
        if(child)
        {
            t = child->add_sister(data);
        }
        else
        {
            child = std::make_shared<Node<_Ty>>(data);
            t = child;
        }

        return t;
    }

    N_ptr add_sister(_Ty *data)
    {
        N_ptr t;
        if(sister)
        {
            t = sister->add_sister(data);
        }
        else
        {
            sister = std::make_shared<Node<_Ty>>(data);
            t = sister;
        }

        return t;
    }
};

template<typename _Ty>
class Tree
{
public:
    typedef Node<_Ty> node_type;
    typedef std::shared_ptr<Node<_Ty>> root_type;

    Tree()
    {
        root = std::make_shared<node_type>();
    }

    root_type root;
};

#endif // TREE_H__
