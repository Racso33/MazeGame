#pragma once

#include "MazeGame.h"

struct HudObject {
    const char* type;
    int evhandler;
    int x, y, w, h;
    bool isHidden;
    bool isHovered;
    int size;
};
struct Button;
typedef void (*ButtonCallback)(Button*);
struct Button {
    HudObject base;
    const char* label;
    ButtonCallback callback;
};

HudObject* InstantiateHudObject(const char* type);
HudObject* HudObject_RegisterNew(const char* type, int size);
void HudObject_RegisterEvent(HudObject* o, const char* type, EventCallback* callback);
void HudObject_SetPosition(HudObject* o, int x, int y, int w, int h);
void HudObjects_Update();
void HudObjects_Draw();

void InitButton();