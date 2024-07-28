#include "MazeGame.h"

struct Button {
    HudObject base;
    const char* label;
};

static void OnDraw(void* d) {
    IODrawRect(30, 30, 200, 200, 255, 255, 255);
}
void InitButton() {
    /* Register gameobject with button 
       OnClick callback, Draw callback 
    */
    Button* h = (Button*)HudObject_RegisterNew("Button", sizeof(Button));
    HudObject_RegisterEvent((HudObject*)h, "DrawHud", OnDraw);
}