#ifndef SGE_SETTINGS_MAIN_H
#define SGE_SETTINGS_MAIN_H
#include "sge_ui/wins.h"
#include "sge_ui/label.h"
#include "sge_ui/button.h"

class sge_settings_main : public Win
{
public:
    sge_settings_main(WContainer *par);

    void Draw() const;
    void Update(const GameTimer& gt, const MouseState &ms) override;

    Button *sc_plus, *sc_minus;
    Label *sc_anounce;

    void* operator new(size_t i)
    {
        return _mm_malloc(i,16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }
};

#endif // SGE_SETTINGS_MAIN_H
