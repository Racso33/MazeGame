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
EventHandler* EventHandler_Register(void* state, EventHandler* e);
void EventHandler_RegisterEvent(EventHandler* t, const char* ev, EventCallback* callback);
EventHandler* EventHandler_Copy(EventHandler* src, EventHandler* target);
void EventHandler_Send(EventHandler* e, const char* ev);
void ProgramEvent_Send(const char* ev);
EventHandler* EventHandler_Get(int i);
int EventHandler_GetIdx(EventHandler* ev);