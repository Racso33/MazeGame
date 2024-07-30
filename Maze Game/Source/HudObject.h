#pragma once

#include "MazeGame.h"

/* TODO:
        Resize event
        create event
        deleting
*/

struct HudObj;
enum HudObjEvent {
    HudObjEventCreate, HudObjEventDestroy, HudObjEventFrame, HudObjEventDraw, HudObjEventClick, HudObjEventHover, HudObjEventResize,
    HudObjEventKeyboard
};
#define HudObjEventCount 8
typedef void (HudObjEventProc)(HudObj* st);
enum HudObjType {
    HudObjTypeButton, HudObjTypeSettingsMenu, HudObjTypeTextBox
};
#define HudObjTypesCount 3
struct HudObj {
    HudObjType type;
    HudObjEventProc* evprocs[HudObjEventCount];
    int x, y, w, h;
    bool isHovered;
    bool hasFocus;
    int size;
};

struct HudObjRef {
    /* a object that is used to reference hudobjects, because otherwise, dangling pointers
       Dangling pointers are now a boogeyman to me, i dont want to see them again
    */
    int ind;
    enum {
        HudObjClass, HudObjInstance
    } namespc;
};

void UpdateHudObjects();
void DrawHudObjects();
HudObjRef InstantiateHudObj(HudObjType type);
HudObjRef RegisterHudObj(HudObjType type, int size);
void HudObjRegEvent(HudObj* st, HudObjEvent e, HudObjEventProc* p);
HudObj* GetHudObj(HudObjRef r);
void HudObjSetPosition(HudObj* o, int x, int y, int w, int h);

void HudInit();

#include "HudObjects\Button.h"
#include "HudObjects\SettingsMenu.h"
#include "HudObjects\TextBox.h"