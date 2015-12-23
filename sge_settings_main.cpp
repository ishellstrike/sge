#include "sge_settings_main.h"
#include "prefecences.h"

sge_settings_main::sge_settings_main(WContainer *par) :
    Win(par)
{
    sc_anounce = new Label(this);
    sc_plus = new Button(this);
    sc_minus = new Button(this);

    sc_anounce->text("Scale");
    sc_plus->size = {20,20};
    sc_plus->pos = sc_anounce->pos + glm::vec2(sc_anounce->size.x, 0);
    sc_plus->text = "^";
    sc_plus->onMouseClick.connect([&](const ClickHandler &mh)->bool{
        sscale *= 2.f;
        if(sscale > 128)
            sscale = 128;
        return true;
    });

    sc_minus->size = {20,20};
    sc_minus->pos = sc_plus->pos + glm::vec2(sc_plus->size.x, 0);
    sc_minus->text = "v";
    sc_minus->onMouseClick.connect([&](const ClickHandler &mh)->bool{
        sscale /= 2.f;
        if(sscale < 1)
            sscale = 1;
        return true;
    });
}

void sge_settings_main::Draw() const
{
    Win::Draw();
}

void sge_settings_main::Update(const GameTimer &gt, const MouseState &ms)
{
    Win::Update(gt, ms);
}
