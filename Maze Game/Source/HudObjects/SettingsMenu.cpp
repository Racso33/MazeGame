#include "..\MazeGame.h"

/* draw background, reposition button when resized */

static void ApplyButton(HudObjButton* b) {
    SettingsMenu* s = (SettingsMenu*)GetHudObj(settingsmenu);
    HudObjTextBox* fovbox = (HudObjTextBox*)GetHudObj(s->fovtext);
    fov = atof(fovbox->buffer);
}
static void NextButton(HudObjButton* b) {
    SettingsMenu* s = (SettingsMenu*)GetHudObj(settingsmenu);
    NewMaze();
}


static void OnCreate(HudObj* o) {
    SettingsMenu* s = (SettingsMenu*)o;
    s->applybutton = InstantiateHudObj(HudObjTypeButton);
    HudObjButtonSet((HudObjButton*)GetHudObj(s->applybutton), ApplyButton, "Apply");
    s->fovtext = InstantiateHudObj(HudObjTypeTextBox);
    s->nextbutton = InstantiateHudObj(HudObjTypeButton);
    HudObjButtonSet((HudObjButton*)GetHudObj(s->nextbutton), NextButton, "Go Next");
}
static void OnResize(HudObj* o) {
    SettingsMenu* s = (SettingsMenu*)o;
    HudObjButton* b = (HudObjButton*)GetHudObj(s->applybutton);
    HudObjButton* b2 = (HudObjButton*)GetHudObj(s->nextbutton);
    HudObjTextBox* fov = (HudObjTextBox*)GetHudObj(s->fovtext);
    
    int bw = 100, bh = 30;
    HudObjSetPosition((HudObj*)b, s->base.x + s->base.w - (bw + 5), s->base.y + s->base.h - (bh + 5), bw, bh);
    HudObjSetPosition((HudObj*)b2, s->base.x + s->base.w - (bw + 5)*2, s->base.y + s->base.h - (bh + 5), bw, bh);

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