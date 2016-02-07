#ifndef SGE_MENU_NEW_GAME_GLOBAL_H
#define SGE_MENU_NEW_GAME_GLOBAL_H
#include "sge_ui/wins.h"
#include "sge_ui/label.h"

class sge_menu_new_game_global : public Win
{
public:
    sge_menu_new_game_global(WContainer *par);

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

#endif // SGE_MENU_NEW_GAME_GLOBAL_H
