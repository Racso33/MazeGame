#include "..\MazeGame.h"

static void OnCreate(HudObj* o) {
    HudObjTextBox* s = (HudObjTextBox*)o;
}
static void OnDraw(HudObj* o) {
    HudObjTextBox* s = (HudObjTextBox*)o;
    IODrawBorderedRect(
        { s->base.x, s->base.y, s->base.w, s->base.h },
        { 255, 255, 255 }, { 120,120,120 }, 
        1
    );
    IODrawText(s->buffer, s->base.x, s->base.y);
}
static void OnKeyboard(HudObj* o) {
    HudObjTextBox* s = (HudObjTextBox*)o;
    s->buffer[s->cursor++] = 'a';
}

void TextBoxInit() {
    SettingsMenu* s = (SettingsMenu*)GetHudObj(RegisterHudObj(HudObjTypeTextBox, sizeof(HudObjTextBox)));
    HudObjRegEvent((HudObj*)s, HudObjEventCreate, OnCreate);
    HudObjRegEvent((HudObj*)s, HudObjEventDraw, OnDraw);
    HudObjRegEvent((HudObj*)s, HudObjEventKeyboard, OnKeyboard);
}