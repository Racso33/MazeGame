#include "MazeGame.h"

static void OnDraw(void* d) {
    Button* t = (Button*)d;
    IODrawRect(t->base.x, t->base.y, t->base.w, t->base.h, t->base.isHovered ? 255 : 200, 255, 255);
    IODrawText(t->label, t->base.x, t->base.y);
}
static void OnClick(void* d) {
    Button* t = (Button*)d;
    if(t->callback) t->callback(t);
}
void InitButton() {
    /* Register gameobject with button 
       OnClick callback, Draw callback 
    */
    Button* h = (Button*)HudObject_RegisterNew("Button", sizeof(Button));
    h->label = "le Button";
    HudObject_RegisterEvent((HudObject*)h, "Draw", OnDraw);
    HudObject_RegisterEvent((HudObject*)h, "Click", OnClick);
}