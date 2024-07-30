#include "..\MazeGame.h"

static void OnCreate(HudObj* o) {
    HudObjLabel* s = (HudObjLabel*)o;
    memset(s->buffer, 0, 256);
}
static void OnDraw(HudObj* o) {
    HudObjLabel* s = (HudObjLabel*)o;
    IODrawText(s->buffer, s->base.x, s->base.y);
}

void LabelInit() {
    HudObjLabel* s = (HudObjLabel*)GetHudObj(RegisterHudObj(HudObjTypeLabel, sizeof(HudObjLabel)));
    HudObjRegEvent((HudObj*)s, HudObjEventCreate, OnCreate);
    HudObjRegEvent((HudObj*)s, HudObjEventDraw, OnDraw);
}