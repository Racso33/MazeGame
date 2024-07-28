#pragma once

#include "MazeGame.h"

struct HudObject {
    const char* type;
    EventHandler* evhandler;
    int size;
};

HudObject* InstantiateHudObject(const char* type);
HudObject* HudObject_RegisterNew(const char* type, int size);
void HudObject_RegisterEvent(HudObject* o, const char* type, EventCallback* callback);

void InitButton();