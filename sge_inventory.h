#ifndef SGE_INVENTORY_H
#define SGE_INVENTORY_H
#include "sge_ui/wins.h"
#include "sge_ui/label.h"

class sge_inventory : public Win
{
public:
    sge_inventory(WContainer *par);

    void Draw() const;
    void Update(const GameTimer& gt, const MouseState &ms) override;

    void* operator new(size_t i)
    {
        return _mm_malloc(i,16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }
};

#endif // SGE_INVENTORY_H
