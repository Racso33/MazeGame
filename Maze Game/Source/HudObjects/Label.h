#pragma once

#include "..\MazeGame.h"

struct HudObjLabel {
    HudObj base;
    char buffer[256];
    int bufferCount;
};

void LabelInit();