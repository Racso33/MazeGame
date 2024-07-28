#include "MazeGame.h"

static void OnDraw(void* d) {
    Button* t = (Button*)d;
    IODrawRect(t->base.x, t->base.y, t->base.w, t->base.h, 255, 255, 255);
    IODrawText(t->label, t->base.x, t->base.y);
}
static void OnClick(void* d) {
    Button* t = (Button*)d;
    IODrawRect(0,0, 640,480, 255, 255, 255);
}
void InitButton() {
    /* Register gameobject with button 
       OnClick callback, Draw callback 
    */
    Button* h = (Button*)HudObject_RegisterNew("Button", sizeof(Button));
    h->label = "le Button";
    HudObject_RegisterEvent((HudObject*)h, "DrawHud", OnDraw);
    HudObject_RegisterEvent((HudObject*)h, "Click", OnClick);
}