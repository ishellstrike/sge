#ifndef SGE_CRAFTING_WINDOW_H
#define SGE_CRAFTING_WINDOW_H
#include "sge_ui/wins.h"
#include "gametimer.h"
#include "sge_ui/list_container.h"
#include "sge_ui/label.h"
#include <map>
#include "core/recipe.h"

class sge_crafting_window : public Win
{
public:
    sge_crafting_window(WContainer *par);

    void Draw() const;
    void Update(const GameTimer& gt, const MouseState &ms) override;
    void Rebuild();
    std::map<std::string, std::vector<std::shared_ptr<Recipe>>> by_category;


    ListContainer *lc;
    Label *info;
    std::shared_ptr<Recipe> selected;
    void Select(const std::shared_ptr<Recipe> &r);
};

#endif // SGE_CRAFTING_WINDOW_H
