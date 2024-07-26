#pragma once

#include "MazeGame.h"

int GetVWall(int col, int y);
int GetHWall(int row, int x);
void SetVWall(int val, int col, int y);
void SetHWall(int val, int row, int x);
void SetMap(int hcells, int vcells);
void GetMap(int* hcells, int* vcells);

/* Raycast but only a certain distance. 
   Or only a certain amount of times. 

   Walk x ammount.
*/

Point Intersection(Point p1, Point p2);
Point Raycast(Point p, double angle);