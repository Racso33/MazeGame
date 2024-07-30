#pragma once

#include "..\MazeGame.h"

struct HudObjTextBox {
    HudObj base;
    char buffer[255];
    int bufferCount;
    int cursor;
};

static void OnCreate(HudObj* o);

void TextBoxInit();