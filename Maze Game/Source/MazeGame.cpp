#include "MazeGame.h"

extern int directions[4][2];
Player player;
double fov;
int raycount;
int timer;

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

int DirTurn(int i, char d) {
    if (d == 'r') i += 1;
    if (d == 'l') i -= 1;
    if (d == 'b') i += 2;
    if (i > 3) {
        i = 0 + i - 4;
    }
    if (i < 0) {
        i = 3 + i - -1;
    }
    return i;
}
double AngDirTurn(double i, double amm) {
    i += amm;
    if (i > M_PI) {
        i = -M_PI + (i - M_PI);
    }
    if (i < -M_PI) {
        i = M_PI + (i - -M_PI);
    }
    return i;
}
double ToAngle(double i) {
    double r = fmod(i, M_PI*2);
    if (r < 0) {
        r = M_PI * 2 + r;
    }
    return r;
}
double AngAngDirTurn(double playerang, double nplayerang) {
    return -1;
}

void DrawMinimap() {
    bool mouse = false;
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
        double rang = ToAngle(player.ang - (((double)raycount*fov) / 2) + ((double)i*fov));
        if (i == 0) inner = rang;
        if (i == 89) outer = rang;
        Point p2 = Raycast(player.pos, rang);
        //printf("%f %f\n", p2.x,p2.y);
        Point mp = Minimap_GetPoint(player.pos.x, player.pos.y);
        Point mp2 = Minimap_GetPoint(p2.x, p2.y);
        IODrawLine(mp, mp2, 1);
    }

    if (mouse) {
        int mx, my;
        IOGetMousePos(&mx, &my);
        Point p = Minimap_GetPointRev(mx, my);
        player.pos = p;
    }
}
void DrawFirstPerson() {
    int i;
    int swidth, sheight;
    IOGetWindowDims(&swidth, &sheight);
    double range = raycount * fov;
    for (i = 0; i < raycount; i++) {
        double rang = ToAngle(player.ang - (((double)raycount * fov) / 2) + ((double)i * fov));
        Point p2 = Raycast(player.pos, rang);
        bool colhit = false;
        if (p2.x - trunc(p2.x)) colhit = true;
        double h = hypot(p2.x - player.pos.x, p2.y - player.pos.y) * cos(rang - player.ang);
        double lh = sheight / h * 0.3;
        double lw = (double)swidth / raycount;
        IODrawRect(swidth/2.0f - (lw*raycount / 2) + i*lw, sheight / 2.0f - lh / 2, lw+1, lh, colhit ? 50 : 30, colhit ? 220 : 170, colhit ? 50 : 30);
    }
}

void NewMaze() {
    int mapw, maph;
    GetMap(&mapw, &maph);
    GenerateMaze({ (double)mapw - 1, (double)maph - 1 });
    player.pos.x = player.pos.y = player.ang = player.dir = player.tang = 0;
    player.hasTarget = false;
    player.pos.x = 0.5; player.pos.y = 0.5;
}
void GameInit() {
    int mapw = 4, maph = 4;
    SetMap(mapw, maph);
    int i, j;
    //for (i = 0; i < 8; i++) {
    //    SetVWall(1, 8, i);
    //}
    //for (i = 0; i < 8; i++) {
    //    SetHWall(1, 8, i);
    //}
    SetHWall(1, 5, 4);
    GenerateMaze({ (double)mapw-1,(double)maph-1});

    memset(&player, 0, sizeof(Player));
    player.movespeed = 0.02;
    player.turnspeed = 0.05;
    player.pos.x = 0.5;
    player.pos.y = 0.5;
    fov = 0.015;
    raycount = 90;
}
void Player_Update(Player* p);
void GameLoop() {
    int mapw, maph;
    GetMap(&mapw, &maph);

    if (!IOGetMousePressed(2) && IOGetMouseScroll() == -1) fov += 0.001;
    if (!IOGetMousePressed(2) && IOGetMouseScroll() == 1) fov -= 0.001;
    if (IOGetMousePressed(2) && IOGetMouseScroll() == 1) raycount += 1;
    if (IOGetMousePressed(2) && IOGetMouseScroll() == -11) raycount -= 1;
    Player_Update(&player);

    /* Draw minimap */
    DrawFirstPerson();
    DrawMinimap();
    timer++;
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
