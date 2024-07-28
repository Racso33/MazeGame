
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
    memcpy_s(result, result->size, tmp, tmp->size);
    result->evhandler = EventHandler_Copy(tmp->evhandler);
    EventHandler_Register(result, result->evhandler);
    if (hudobjectsCount + 1 > hudobjectsMem) {
        hudobjectsMem += 10;
        hudobjects = (HudObject**)realloc(hudobjects, sizeof(HudObject*) * hudobjectsMem);
        memset(hudobjects + hudobjectsCount, 0, sizeof(HudObject*) * 10);
    }
    hudobjects[hudobjectsCount] = result;
    return hudobjects[hudobjectsCount++];
}
HudObject* HudObject_RegisterNew(const char* type, int size) {
    if (hudobjectstempsCount + 1 > hudobjectstempsMem) {
        hudobjectstempsMem += 10;
        hudobjectstemps = (HudObject**)realloc(hudobjectstemps, sizeof(HudObject*) * hudobjectstempsMem);
        memset(hudobjectstemps + hudobjectstempsCount, 0, sizeof(HudObject*) * 10);
        hudobjectstype = (const char**)realloc(hudobjectstype, sizeof(const char*) * hudobjectstempsMem);
        memset(hudobjectstype + hudobjectstempsCount, 0, sizeof(const char*) * 10);
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
}

/* a button */