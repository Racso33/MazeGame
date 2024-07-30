#include "..\MazeGame.h"

/* draw background, reposition button when resized */

static void OnCreate(HudObj* o) {
    SettingsMenu* s = (SettingsMenu*)o;
    s->applybutton = InstantiateHudObj(HudObjTypeButton);
    HudObjButtonSet((HudObjButton*)GetHudObj(s->applybutton), 0, "Apply");
    s->fovtext = InstantiateHudObj(HudObjTypeTextBox);
    sprintf_s(((HudObjTextBox*)GetHudObj(s->fovtext))->buffer, "Hi");
}
static void OnResize(HudObj* o) {
    SettingsMenu* s = (SettingsMenu*)o;
    HudObjButton* b = (HudObjButton*)GetHudObj(s->applybutton);
    HudObjTextBox* fov = (HudObjTextBox*)GetHudObj(s->fovtext);
    
    int bw = 50, bh = 30;
    HudObjSetPosition((HudObj*)b, s->base.x + s->base.w - (bw + 5), s->base.y + s->base.h - (bh + 5), bw, bh);

    HudObjSetPosition((HudObj*)fov, s->base.x+1, s->base.y+1, s->base.w-2, 50);

}
static void OnDraw(HudObj* o) {
    SettingsMenu* s = (SettingsMenu*)o;
    IODrawRect(s->base.x, s->base.y, s->base.w, s->base.h, 200, 50, 200);
}

void SettingsMenuInit() {
    SettingsMenu* s = (SettingsMenu*)GetHudObj(RegisterHudObj(HudObjTypeSettingsMenu, sizeof(SettingsMenu)));
    HudObjRegEvent((HudObj*)s, HudObjEventCreate, OnCreate);
    HudObjRegEvent((HudObj*)s, HudObjEventResize, OnResize);
    HudObjRegEvent((HudObj*)s, HudObjEventDraw, OnDraw);
}