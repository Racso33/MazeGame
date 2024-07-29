
#include "MazeGame.h"

HudObject** hudobjects;
int hudobjectsCount;
int hudobjectsMem;

HudObject** hudobjectstemps;
const char** hudobjectstype;
int hudobjectstempsCount;
int hudobjectstempsMem;

/* Registering new hudobjects
   Instantiating */
int HudObjects_GetTypeIdx(const char* type) {
    for (int i = 0; i < hudobjectstempsCount; i++) {
        if (!strcmp(hudobjectstype[i], type)) {
            return i;
        }
    }
    return -1;
}
HudObject* HudObjects_GetTemplate(const char* type) {
    int idx = HudObjects_GetTypeIdx(type);
    return idx != -1 ? hudobjectstemps[idx] : 0;
}
HudObject* InstantiateHudObject(const char* type) {
    /* allocating and nulling, copy template object */
    HudObject* tmp = HudObjects_GetTemplate(type);
    if (!tmp) return 0;
    HudObject* result = (HudObject*)malloc(tmp->size);
    /* copy stuff from tmp */
    memcpy(result, tmp, tmp->size);
    /* register new
       copy
       set index
    */
    EventHandler* evhandler = EventHandler_RegisterNew(result);
    EventHandler_Copy(evhandler, EventHandler_Get(tmp->evhandler));
    result->evhandler = EventHandler_GetIdx(evhandler);
    /*if (!hudobjects) {
        hudobjectsMem = 10;
        hudobjects = (HudObject**)malloc(sizeof(HudObject*) * hudobjectsMem);
    }*/
    if (hudobjectsCount + 1 > hudobjectsMem) {
        hudobjectsMem += 10;
        hudobjects = (HudObject**)realloc(hudobjects, sizeof(HudObject*) * hudobjectsMem);
        //memset(hudobjects + hudobjectsCount, 0, sizeof(HudObject*) * 10);
    }
    hudobjects[hudobjectsCount++] = result;
    EventHandler_Send(evhandler, "Create");
    return result;
}
HudObject* HudObject_RegisterNew(const char* type, int size) {
    if (hudobjectstempsCount + 1 > hudobjectstempsMem) {
        hudobjectstempsMem += 10;
        //hudobjectstemps = (HudObject**)realloc(hudobjectstemps, sizeof(HudObject*) * hudobjectstempsMem);
        hudobjectstemps = (HudObject**)malloc(sizeof(HudObject*) * hudobjectstempsMem);
        //memset(hudobjectstemps + hudobjectstempsCount, 0, sizeof(HudObject*) * 10);
        //hudobjectstype = (const char**)realloc(hudobjectstype, sizeof(const char*) * hudobjectstempsMem);
        hudobjectstype = (const char**)malloc(sizeof(const char*) * hudobjectstempsMem);
        //memset(hudobjectstype + hudobjectstempsCount, 0, sizeof(const char*) * 10);
    }
    HudObject* res = (HudObject*)malloc(size);
    memset(res, 0, size);
    res->type = type;
    res->size = size;

    hudobjectstemps[hudobjectstempsCount] = res;
    hudobjectstype[hudobjectstempsCount] = res->type;
    return hudobjectstemps[hudobjectstempsCount++];
}
void HudObject_RegisterEvent(HudObject* o, const char* type, EventCallback* callback) {
    if (!o->evhandler) {
        o->evhandler = (EventHandler*)malloc(sizeof(EventHandler));
        memset(o->evhandler, 0, sizeof(EventHandler));
    }
    EventHandler_RegisterEvent(o->evhandler, type, callback);
}
void HudObject_SetPosition(HudObject* o, int x, int y, int w, int h) {
    o->x = x;
    o->y = y;
    o->w = w;
    o->h = h;
    EventHandler_Send(o->evhandler, "Resize");
}
bool RectOverlap(Point p, Rect r) {
    return p.x >= r.x && p.x <= r.x + r.w
        && p.y >= r.y && p.y <= r.y + r.h;
}
void HudObjects_Update() {
    /* send click events */
    int mx, my;
    IOGetMousePos(&mx, &my);
    Point m = { mx, my };
    HudObject* hov = 0;
    for (int i = 0; i < hudobjectsCount; i++) {
        Rect rect = { hudobjects[i]->x, hudobjects[i]->y, hudobjects[i]->w, hudobjects[i]->h };
        if (RectOverlap(m, rect)) {
            hov = hudobjects[i];
        }
        hudobjects[i]->isHovered = false;
    }
    if (hov) {
        hov->isHovered = true;
        if (IOGetMousePressed(0)) {
            EventHandler_Send(hov->evhandler, "Click");
        }
    }
}
void HudObjects_Draw() {
    for (int i = 0; i < hudobjectsCount; i++) {
        EventHandler_Send(hudobjects[i]->evhandler, "Draw");
    }
}

/* a button */