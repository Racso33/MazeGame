#pragma once

#include "MazeGame.h"

typedef void (EventCallback)(void*);
struct EventHandler {
    void* state;
    EventCallback** events;
    const char** eventTypes;
    int eventsCount;
    int eventsMem;
};

EventHandler* EventHandler_RegisterNew(void* state);
void EventHandler_RegisterEvent(EventHandler* t, const char* ev, EventCallback callback);
void EventHandler_Send(EventHandler* e, const char* ev);
void ProgramEvent_Send(const char* ev);