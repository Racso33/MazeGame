#pragma once

#include "MazeGame.h"

struct SettingsMenu {
    HudObject base;
    Button* ApplyButton;
};

void SettingsMenu_Init();