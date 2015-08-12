#ifndef SGE_TEXTURE_LAB_H
#define SGE_TEXTURE_LAB_H
#include "sge_ui/wins.h"
#include "sge_ui/graph.h"
#include "sge_ui/label.h"

class sge_texture_lab : public Win
{
public:
    sge_texture_lab(WContainer *par);

    void Draw() const;
    void Update();

    void* operator new(size_t i)
    {
        return _mm_malloc(i,16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }
};

#endif // SGE_TEXTURE_LAB_H
