#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <math.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "GameIO.h"
#include "Map.h"
#include "HudObject.h"

struct Player {
    Point pos;
    Point target;
    int dir;
    bool hasTarget;
    double ang;
    double tang;
    int timer;
    double turnspeed;
    double movespeed;
    int movecooldown;
};

void NewMaze();
void GenerateMaze(Point origin);
int GetColumns();
int GetRows();
double ToAngle(double a);
double AngleDiff(double n1, double n2);

extern Player player;
extern int directions[4][2];
extern double fov;
extern HudObjRef settingsmenu;
extern double wallheight;