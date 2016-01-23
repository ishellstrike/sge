#ifndef SGE_CHEST_WINDOW_H
#define SGE_CHEST_WINDOW_H
#include "sge_ui/wins.h"
#include "gametimer.h"
#include "sge_ui/label.h"
#include "sge_ui/list_container.h"
#include "sge_ui/table.h"

class Object;
class Chest;

class sge_chest_window : public Win
{
public:
    sge_chest_window(WContainer *par);
    Table *lc;
    Button *take, *takeall;

    void Draw() const;
    void Update(const GameTimer& gt, const MouseState &ms) override;

    std::weak_ptr<Object> linked;
    std::weak_ptr<Object> hero;
    Chest *c;
    void Link(std::shared_ptr<Object> &o, std::shared_ptr<Object> &hero);
    void Unlink();
private:
    bool TakeAll();
    bool TakeSelected();
};

#endif // SGE_CHEST_WINDOW_H
