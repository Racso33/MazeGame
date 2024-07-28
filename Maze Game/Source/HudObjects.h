#pragma once

#include "MazeGame.h"

struct HudObject {
    const char* type;
    EventHandler* evhandler;
    int x, y, w, h;
    bool isHidden;
    int size;
};
struct Button {
    HudObject base;
    const char* label;
};

HudObject* InstantiateHudObject(const char* type);
HudObject* HudObject_RegisterNew(const char* type, int size);
void HudObject_RegisterEvent(HudObject* o, const char* type, EventCallback* callback);
void HudObject_SetPosition(HudObject* o, int x, int y, int w, int h);

void InitButton();