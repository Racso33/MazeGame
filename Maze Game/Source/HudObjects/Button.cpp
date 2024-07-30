#include "..\MazeGame.h"

static void OnDraw(HudObj* o) {
    HudObjButton* b = (HudObjButton*)o;
    IODrawRect(b->base.x, b->base.y, b->base.w, b->base.h, b->base.isHovered ? 255 : 200, 255, 255);
    IODrawText(b->label, b->base.x, b->base.y);
}
static void OnClick(HudObj* o) {
    HudObjButton* b = (HudObjButton*)o;
    IODrawRect(0,0, 640, 480, 255, 255, 255);
}

void ButtonInit() {
    HudObjRef r = RegisterHudObj(HudObjTypeButton, sizeof(HudObjButton));
    HudObj* m = GetHudObj(r);
    HudObjRegEvent(m, HudObjEventDraw, OnDraw);
    HudObjRegEvent(m, HudObjEventClick, OnClick);
}

void HudObjButtonSet(HudObjButton* b, HudObjButtonCallback* c, const char* e) {
    b->clickcallback = c;
    b->label = e;
}