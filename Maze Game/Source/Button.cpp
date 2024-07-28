#include "MazeGame.h"

static void OnDraw(void* d) {
    Button* t = (Button*)d;
    IODrawRect(t->base.x, t->base.y, t->base.w, t->base.h, 255, 255, 255);
    IODrawText(t->label, t->base.x, t->base.y);
}
void InitButton() {
    /* Register gameobject with button 
       OnClick callback, Draw callback 
    */
    Button* h = (Button*)HudObject_RegisterNew("Button", sizeof(Button));
    h->label = "le Button";
    HudObject_RegisterEvent((HudObject*)h, "DrawHud", OnDraw);
}