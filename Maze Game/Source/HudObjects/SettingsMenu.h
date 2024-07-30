#pragma once

#include "..\MazeGame.h"

struct SettingsMenu {
    HudObj base;
    HudObjRef applybutton;
    HudObjRef nextbutton;
    HudObjRef fovfield, fovlabel;
    HudObjRef rotspeedfield, rotspeedlabel;
    HudObjRef movspeedfield, movspeedlabel;
    HudObjRef wallhfield, wallhlabel;
};

void SettingsMenuLoadFields(SettingsMenu* s);