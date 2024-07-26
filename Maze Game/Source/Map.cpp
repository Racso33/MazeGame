#include "MazeGame.h"

int* hwalls, * vwalls;
int HCellCount, VCellCount;

int GetVWall(int col, int y) {
    if (col <= 0 || col >= HCellCount || y < 0 || y >= VCellCount) return 1;
    return vwalls[col + y * HCellCount];
}
int GetHWall(int row, int x) {
    if (row <= 0 || row >= VCellCount || x < 0 || x >= HCellCount) return 1;
    return hwalls[row + x * VCellCount];
}
void SetVWall(int val, int col, int y) {
    vwalls[col + y * HCellCount] = val;
}
void SetHWall(int val, int row, int x) {
    hwalls[row + x * VCellCount] = val;
}
void SetMap(int hcells, int vcells) {
    HCellCount = hcells;
    VCellCount = vcells;

    vwalls = (int*)malloc(sizeof(int) * ((HCellCount + 1) * VCellCount));
    memset(vwalls, 0, sizeof(int) * ((HCellCount + 1) * VCellCount));
    hwalls = (int*)malloc(sizeof(int) * ((VCellCount + 1) * HCellCount));
    memset(hwalls, 0, sizeof(int) * ((VCellCount + 1) * HCellCount));
}
void GetMap(int* hcells, int* vcells) {
    *hcells = HCellCount;
    *vcells = VCellCount;
}

Point StepColumn(Point p, double angle) {
    /* how much to move for next column */

    /* return p + ammount to move, p + (move * tangent) */

    double r = tan(angle);
    double cp = p.x - trunc(p.x);
    double nc = 1;
    int ys;
    if (angle < M_PI / 2 || (angle <= M_PI*2 && angle > M_PI + M_PI / 2)) nc = 1;
    else if (cp == 0) nc = -1;
    else nc = 0;
    double dist = nc - cp;
    Point res = { p.x + (dist), p.y + (dist * r)};
    return res;
}
Point StepRow(Point p, double angle) {
    double t = tan(angle);
    double r = 1 / (t != 0 ? t : -0.00001);
    double cp = p.y - trunc(p.y);
    double nc = 1;
    int xs;
    if (angle >= 0 && angle < M_PI) nc = 1;
    else if(cp == 0) nc = -1;
    else nc = 0;
    double dist = nc - cp;
    Point res = { p.x + (dist * r), p.y + dist };
    return res;
}
Point Shortest(Point p, Point p1, Point p2) {
    double h1 = hypot(p1.x - p.x, p1.y - p.y);
    double h2 = hypot(p2.x - p.x, p2.y - p.y);
    if (abs(h1) < abs(h2)) return p1;
    else return p2;
}

/* Point of impact
   From a point, going to a line */
Point RaycastEx(Point p, double angle, int maxsteps, bool* error) {
    /* Possible bug: 
        Rounding errors 
    */
    /* step along cols and rows. If a wall is there, return. */
    Point p1 = p, p2 = p;
    int w1 = 0, w2 = 0;
    int steps = 0;
    while((!w1 || !w2) && (maxsteps == -1 || steps <= maxsteps)) {
        if(!w1) p1 = StepColumn(p1, angle);
        if(!w2) p2 = StepRow(p2, angle);
        /* pick shortest one */
        w1 = GetVWall(p1.x, p1.y);
        w2 = GetHWall(p2.y, p2.x);
        /* if p is on wall: return
           A system where you can use a point to get a wall.
           Or, more  so, translate point into a wall index
           Horizontal Wall index = Row, x
           Vertical Wall index = Col, y
        */
        steps++;
    }
    if (w1 && !w2) return p1;
    else if (!w1 && w2) return p2;
    else return Shortest(p, p1, p2);
    if(error) *error = true;
    return {};
}
Point Raycast(Point p, double angle) {
    return RaycastEx(p, angle, -1, 0);
}
bool Intersection(Point p1, Point p2) {
    Point dp = { p2.x - p1.x, p2.y - p1.y };
    double angle = atan2(dp.y, dp.x);
    int steps = ceil(hypot(dp.x, dp.y));
    bool error;
    Point res = RaycastEx(p1, angle, steps, &error);
    if (error) return false;
    return true;
}