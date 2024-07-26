#include "MazeGame.h"

Point player;
double playerang;
double fov;
int raycount;

int MinimapScale = 10;
int MinimapWallScale = 1;

Rect Minimap_GetVWall(int col, int y) {
    return { col * MinimapScale, y * MinimapScale, MinimapWallScale, MinimapScale };
}
Rect Minimap_GetHWall(int row, int x) {
    return { x * MinimapScale, row * MinimapScale, MinimapScale, MinimapWallScale };
}
Point Minimap_GetPoint(double x, double y) {
    return { x * MinimapScale, y * MinimapScale };
}
Point Minimap_GetPointRev(int x, int y) {
    return { (double)x / MinimapScale, (double)y / MinimapScale };
}

void DrawMinimap() {
    int i, j;
    int hcellcount, vcellcount;
    GetMap(&hcellcount, &vcellcount);
    IODrawRect(0, 0, hcellcount * MinimapScale, vcellcount * MinimapScale, 30, 30, 30);
    /* vertical */
    for(j=0; j < vcellcount; j++) {
        for (i = 0; i < hcellcount + 1; i++) {
            Rect r = Minimap_GetVWall(i, j);
            int w = GetVWall(i, j);
            if(w) IODrawRect(r.x, r.y, r.w, r.h, 220, 220, 220);
        }
    }

    for (i = 0; i < hcellcount; i++) {
        for (j = 0; j < vcellcount + 1; j++) {
            Rect r = Minimap_GetHWall(j, i);
            int w = GetHWall(j, i);
            if(w) IODrawRect(r.x, r.y, r.w, r.h, 220, 220, 220);
        }
    }

    double outer, inner;
    /* draw field of vision */
    for (i = 0; i < raycount; i++) {
        double rang = playerang - (((double)raycount*fov) / 2) + ((double)i*fov);
        if (i == 0) inner = rang;
        if (i == 89) outer = rang;
        Point p2 = Raycast(player, rang);
        //printf("%f %f\n", p2.x,p2.y);
        Point mp = Minimap_GetPoint(player.x, player.y);
        Point mp2 = Minimap_GetPoint(p2.x, p2.y);
        IODrawLine(mp, mp2, 1);
    }

    int mx, my;
    IOGetMousePos(&mx, &my);
    Point p = Minimap_GetPointRev(mx, my);
    player = p;
}
void DrawFirstPerson() {
    int i;
    int swidth, sheight;
    IOGetWindowDims(&swidth, &sheight);
    for (i = 0; i < raycount; i++) {
        double rang = playerang - (((double)raycount * fov) / 2) + ((double)i * fov);
        Point p2 = Raycast(player, rang);
        double h = hypot(p2.x - player.x, p2.y - player.y);
        int lh = sheight / h * 1.5;
        int lw = 16;
        IODrawRect(swidth/2 - (lw*raycount / 2) + i * lw, sheight / 2 - lh / 2, lw, lh, fmax(50 - h*10, 0), fmax(220 - h*10, 0), fmax(50 - h*10, 0));
    }
}

void GameInit() {
    SetMap(32, 32);
    int i, j;
    //for (i = 0; i < 8; i++) {
    //    SetVWall(1, 8, i);
    //}
    //for (i = 0; i < 8; i++) {
    //    SetHWall(1, 8, i);
    //}
    SetHWall(1, 5, 4);

    player = { 3.4,3.5 };
    playerang = 0;
    fov = 0.02;
    raycount = 90;
}
void GameLoop() {
    if (IOKeyIsDown(SDLK_j)) {
        playerang += 0.01;
        if (playerang > M_PI*2) playerang = 0;
    }
    /* Draw minimap */
    DrawMinimap();
    DrawFirstPerson();
}

int main(int argc, char** argv) {
    IOInit();
    GameInit();
    while (!IOShouldClose()) {
        IOFrame();

        GameLoop();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}
