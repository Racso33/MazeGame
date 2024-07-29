#include "MazeGame.h"

void ApplyButton_Click(Button* b) {
    IODrawRect(0, 0, 640, 480, 255, 255, 255);
}
void SettingsMenu_Create(void* s) {
    SettingsMenu* t = (SettingsMenu*)s;
    t->ApplyButton = (Button*)InstantiateHudObject("Button");
    t->ApplyButton->callback = ApplyButton_Click;
    t->ApplyButton->label = "Apply";
}
void SettingsMenu_Resize(void* s) {
    SettingsMenu* t = (SettingsMenu*)s;
    int bw = 100;
    int bh = 50;
    HudObject_SetPosition((HudObject*)t->ApplyButton, t->base.x + t->base.w - bw, t->base.y + t->base.h - bh, bw - 5, bh - 5);
}
void SettingsMenu_Draw(void* s) {
    SettingsMenu* t = (SettingsMenu*)s;
    IODrawRect(t->base.x, t->base.y, t->base.w, t->base.h, 84, 17, 135);
}

void SettingsMenu_Init() {
    /* applybutton class? 
       Create event? 
       resize event
    */
    HudObject* s = HudObject_RegisterNew("SettingsMenu", sizeof(SettingsMenu));
    HudObject_RegisterEvent(s, "Create", SettingsMenu_Create);
    HudObject_RegisterEvent(s, "Resize", SettingsMenu_Resize);
    HudObject_RegisterEvent(s, "Draw", SettingsMenu_Draw);
}