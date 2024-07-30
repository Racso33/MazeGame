#include "MazeGame.h"

HudObj* hudobjectsclass[HudObjTypesCount];

HudObj** hudobjects;
int hudobjectsCount;
int hudobjectsMem;

HudObjRef RegisterHudObj(HudObjType type, int size) {
    hudobjectsclass[type] = (HudObj*)malloc(size);
    memset(hudobjectsclass[type], 0, size);
    hudobjectsclass[type]->type = type;
    hudobjectsclass[type]->size = size;
    return { type, HudObjRef::HudObjClass };
}
HudObj* GetHudObj(HudObjRef r) {
    HudObj* res = 0;
    if (r.namespc == r.HudObjClass) {
        return hudobjectsclass[r.ind];
    }
    else if (r.namespc == r.HudObjInstance) {
        return hudobjects[r.ind];
    }
    return 0;
}
void HudObjRegEvent(HudObj* st, HudObjEvent e, HudObjEventProc* p) {
    st->evprocs[e] = p;
}
HudObjRef InstantiateHudObj(HudObjType type) {
    if (hudobjectsCount + 1 > hudobjectsMem) {
        hudobjectsMem += 10;
        hudobjects = (HudObj**)realloc(hudobjects, sizeof(HudObj*) * hudobjectsMem);
    }
    HudObj* objclass = hudobjectsclass[type];
    if (!objclass) {
        fprintf(stderr, "[ERROR] HudObjectClass: %d is not registered", type);
        exit(1);
    }
    HudObj* res = (HudObj*)malloc(objclass->size);
    memcpy(res, objclass, objclass->size);
    hudobjects[hudobjectsCount] = res;
    return { hudobjectsCount++, HudObjRef::HudObjInstance };
}
void HudObjSetPosition(HudObj* o, int x, int y, int w, int h) {
    o->x = x;
    o->y = y;
    o->w = w;
    o->h = h;
}
bool RectOverlap(int x, int y, Rect r) {
    return x >= r.x && x <= r.x + r.w
        && y >= r.y && y <= r.y + r.h;
}
void UpdateHudObjects() {
    int mx, my;
    IOGetMousePos(&mx, &my);
    HudObj* hov = 0;
    for (int i = 0; i < hudobjectsCount; i++) {
        HudObj* obj = hudobjects[i];
        Rect objr = { obj->x, obj->y, obj->w, obj->h };
        obj->isHovered = false;
        if (RectOverlap(mx, my, objr)) {
            hov = obj;
        }
    }
    if (hov) {
        hov->isHovered = true;
        if (IOGetMousePressed(0)) {
            HudObjEventProc* proc = hov->evprocs[HudObjEventClick];
            if (proc) proc(hov);
        }
    }
}
void DrawHudObjects() {
    for (int i = 0; i < hudobjectsCount; i++) {
        HudObj* oi = hudobjects[i];
        HudObjEventProc* op = oi->evprocs[HudObjEventDraw];
        if(op) op(oi);
    }
}
void ButtonInit();
void HudInit() {
    ButtonInit();
}