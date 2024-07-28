
#include "MazeGame.h"

/* array of event handlers */

/* Create event handler
   Adding callbacks to events
   */

/* global eventhandlers here */
EventHandler* eventhandlers;
int eventhandlersCount;
int eventhandlersMem;

EventHandler* EventHandler_RegisterNew(void* state) {
    if (eventhandlersCount + 1 > eventhandlersMem) {
        eventhandlersMem += 10;
        eventhandlers = (EventHandler*)realloc(eventhandlers, sizeof(EventHandler*) * eventhandlersMem);
        memset(eventhandlers + eventhandlersCount, 0, sizeof(EventHandler) * 10);
    }
    return &eventhandlers[eventhandlersCount++];
}
void EventHandler_RegisterEvent(EventHandler* t, const char* ev, EventCallback callback) {
    if (t->eventsCount + 1 > t->eventsMem) {
        t->eventsMem += 10;
        t->events = (EventCallback**)realloc(t->events, sizeof(EventCallback*) * t->eventsMem);
        t->eventTypes = (const char**)realloc(t->eventTypes, sizeof(const char**) * t->eventsMem);
    }
    t->eventTypes[t->eventsCount] = ev;
    t->events[t->eventsCount] = callback;
    t->eventsCount++;
}
int EventHandler_GetTypeIdx(EventHandler* t, const char* ev) {
    for (int i = 0; i < t->eventsCount; i++) {
        if (!strcmp(t->eventTypes[i], ev)) {
            return i;
        }
    }
    return -1;
}
EventCallback* EventHandler_GetCallback(EventHandler* t, const char* ev) {
    int idx = EventHandler_GetTypeIdx(t, ev);
    return idx != -1 ? t->events[idx] : 0;
}
void EventHandler_Send(EventHandler* e, const char* ev) {
    EventCallback* callback = EventHandler_GetCallback(e, ev);
    if(callback) callback(e->state);
}
void ProgramEvent_Send(const char* ev) {
    for (int i = 0; i < eventhandlersCount; i++) {
        EventHandler_Send(&eventhandlers[i], ev);
    }
}