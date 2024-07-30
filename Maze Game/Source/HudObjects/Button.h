#pragma once

struct HudObjButton;
typedef void (HudObjButtonCallback)(HudObjButton* st);
struct HudObjButton {
    HudObj base;
    const char* label;
    HudObjButtonCallback* clickcallback;
};

void HudObjButtonSet(HudObjButton* o, HudObjButtonCallback* c, const char* l);