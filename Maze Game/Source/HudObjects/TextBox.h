#pragma once

#include "..\MazeGame.h"

struct HudObjTextBox {
    HudObj base;
    char buffer[256];
    int bufferCount;
    int cursor;
};

void TextBoxInit();